#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "tty_color.h"

#define LOG_TAG	"server"

#define TEST_SOCKET	"\0test_socket"

void *handle_client(void *arg)
{
	logd("connected sock fd is %d", *(int *)arg);
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
