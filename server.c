#include <stdio.h>
#include <poll.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "tty_color.h"

#define LOG_TAG	"server"

#define TEST_SOCKET	"\0test_socket"

void *handle_client(void *arg)
{
	struct msghdr msg;
	struct iovec iv;
	struct cmsghdr *cmsg;
	char cmsgbuf[CMSG_SPACE(sizeof(int))];
	char buf[1024];
	struct pollfd pfd;
	int sock = *(int *)arg;
	int ret;
	int fd;

	logd("connected sock fd is %d", sock);

	pfd.fd = sock;
	pfd.events = POLLIN | POLLERR | POLLHUP | POLLNVAL;

	ret = poll(&pfd, 1, 10 * 1000);
	if (ret < 0) {
		loge("poll() failed");
	} else if (ret > 0) {
		if (pfd.revents & POLLHUP) {
			logi("poll() got event POLLHUP");
		}
		if (pfd.revents & POLLERR) {
			logi("poll() got event POLLERR");
		}
		if (pfd.revents & POLLNVAL) {
			logi("poll() got event POLLNVAL");
		}
		if (pfd.revents & POLLIN) {
			logi("poll() got event POLLIN");
			memset(&msg, 0, sizeof(msg));
			memset(&cmsgbuf, 0, sizeof(cmsgbuf));
			memset(&buf, 0, sizeof(buf));

			iv.iov_base = buf;
			iv.iov_len = sizeof(buf);

			msg.msg_iov = &iv;
			msg.msg_iovlen = 1;
			msg.msg_control = cmsgbuf;
			msg.msg_controllen = sizeof(cmsgbuf);

			ret = recvmsg(sock, &msg, 0);
			if (ret < 0) {
				loge("recvmsg() failed");
			}
			if (ret == 0) {
				logi("socket is closed");
			} else {
				logi("recvmsg() got %d\n", ret);
			}

			fd = -1;
			for (cmsg = CMSG_FIRSTHDR(&msg); cmsg;
					cmsg = CMSG_NXTHDR(&msg, cmsg)) {
				if (cmsg->cmsg_level == SOL_SOCKET &&
						cmsg->cmsg_type == SCM_RIGHTS) {
					memcpy(&fd, CMSG_DATA(cmsg), sizeof(int));
				}
			}
		}
	}
}

int main(int argc, char *argv[])
{
	struct sockaddr_un addr;
	int server_sock;
	int client_sock;
	pthread_t handle_thread;

	server_sock = socket(AF_LOCAL, SOCK_SEQPACKET, 0);
	if (server_sock < 0) {
		loge("socket() failed");
		return -1;
	} else {
		logd("socket() success");
	}

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	memcpy(addr.sun_path, TEST_SOCKET, sizeof(TEST_SOCKET));

	if (bind(server_sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		loge("bind() failed");
		goto fail;	
	} else {
		logd("bind() success");
	}

	if (listen(server_sock, 5) < 0) {
		loge("listen() failed");
		goto fail;
	} else {
		logd("listen() success");
	}

	while (1) {
		client_sock = accept(server_sock, NULL, NULL);
		pthread_create(&handle_thread, NULL, handle_client,
				(void *)&client_sock);
		pthread_detach(handle_thread);
	}
	close(server_sock);

	return 0;
fail:
	close(server_sock);
	return -1;
}
