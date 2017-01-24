#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "tty_color.h"

#define LOG_TAG	"client"

#define TEST_SOCKET	"\0test_socket"

int main(int argc, char *argv[])
{
	struct sockaddr_un addr;
	int server_sock;
	int client_sock;

	client_sock = socket(AF_LOCAL, SOCK_SEQPACKET, 0);
	if (client_sock < 0) {
		loge("socket() failed");
		return -1;
	} else {
		logd("socket() success");
	}

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	memcpy(addr.sun_path, TEST_SOCKET, sizeof(TEST_SOCKET));

	server_sock = connect(client_sock, (struct sockaddr *)&addr,
			sizeof(addr));
	if (server_sock < 0) {
		loge("connect() failed");
	} else {
		loge("connect() success");
	}

	close(server_sock);
	close(client_sock);
fail:
	close(server_sock);
	close(client_sock);
	return -1;
}
