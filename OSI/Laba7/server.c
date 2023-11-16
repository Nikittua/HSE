#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <assert.h>

static inline unsigned proccess_pids(void *data)
{
	char *pid = strtok(data, "\n"), system_buf[100];
	FILE *pipe;
	char tmp[3000];
	unsigned offset = 0;

	while (pid) {
		snprintf(system_buf, sizeof(system_buf), "ps -o nice='' -p %s", pid);
		pipe = popen(system_buf, "r");

		offset += fread(tmp + offset, 1, sizeof(tmp) - offset, pipe);
		pclose(pipe);
		assert(offset < 3000);

		pid = strtok(NULL, "\n");
	}

	memcpy(data, tmp, offset);

	return offset;
}

int main(void)
{
	int sock, ret;
	struct sockaddr_in addr, client = { };
	char buf[3000] = { };
	unsigned client_size;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		perror("Failed to create sock");
		exit(10);
	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(0x1234);

	ret = bind(sock, (struct sockaddr *) &addr, sizeof(addr));
	if (ret) {
		perror("Failed to bind sock");
		exit(10);
	}

	client_size = sizeof(client);

	ret = recvfrom(sock, buf, sizeof(buf), MSG_WAITALL, (struct sockaddr *) &client, &client_size);
	if (ret <= 0) {
		perror("Failed to recv from sock");
		exit(10);
	}

	ret = proccess_pids(buf);

	ret = sendto(sock, buf, ret, 0, (struct sockaddr *) &client, sizeof(client));
	if (ret <= 0) {
		perror("Failed to send");
		exit(10);
	}
	shutdown(sock, SHUT_RDWR);
}

