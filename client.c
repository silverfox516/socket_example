#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "tty_color.h"

#define LOG_TAG	"client"

#define TEST_SOCKET	"\0test_socket"

int main(int argc, char *argv[])
{
	struct sockaddr_un addr;
	int sock;
	int ret;

	sock = socket(PF_LOCAL, SOCK_SEQPACKET, 0);
	if (sock < 0) {
		loge("socket() failed");
		return -1;
	} else {
		logd("socket() success");
	}

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	memcpy(addr.sun_path, TEST_SOCKET, sizeof(TEST_SOCKET));

	ret = connect(sock, (struct sockaddr *)&addr,
			sizeof(addr));
	if (ret < 0) {
		loge("connect() failed");
	} else {
		loge("connect() success");
	}

	struct msghdr msg;
	struct iovec iv;
	char buf[1024];

	memset(&msg, 0, sizeof(msg));
	memset(&buf, 0, sizeof(buf));

	iv.iov_base = buf;
	iv.iov_len = sizeof(buf);

	msg.msg_iov = &iv;
	msg.msg_iovlen = 1;

	ret = sendmsg(sock, &msg, 0);
	if (ret < 0) {
		perror("");
		loge("sendmsg() failed");
	}
	if (ret == 0) {
		logi("socket is closed");
	} else {
		logd("sended %d", ret);
	}

	close(sock);
fail:
	close(sock);
	return -1;
}
