#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(void)
{
	int sock, ret;
	struct sockaddr_in addr, server;
	char buf[3000];
	FILE *pipe;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		perror("Failed to create sock");
		exit(10);
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(0x1234);

	pipe = popen("ps -u root | awk '{print $1}' | sed '1d'", "r");
	ret = fread(buf, 1, sizeof(buf), pipe);

	ret = sendto(sock, buf, ret, 0, (struct sockaddr *) &server, sizeof(server));
	if (ret < 0) {
		perror("Failed to send data");
		exit(10);
	}

	ret = recvfrom(sock, buf, sizeof(buf), 0, NULL, NULL);
	if (ret < 0) {
		perror("Failed to recv data");
		exit(10);
	}

	buf[ret] = 0x0;

	printf("Server reply %s\n", buf);
}
