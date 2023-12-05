#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
    int sock_id, ret;
    struct sockaddr_in addr, server;
    char buf[3000];
    unsigned server_size;
    FILE *pipe;

    sock_id = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_id < 0) {
        perror("Failed to get sock_id");
        exit(1);
    }

    // Явная привязка клиентского сокета к порту 0
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(0);

    ret = bind(sock_id, (struct sockaddr*)&addr, sizeof(addr));
    if (ret < 0) {
        perror("Failed to bind sock_id");
        exit(1);
    }

    // Затем устанавливаем адрес сервера и порт
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY; // Замените на реальный IP-адрес сервера
    server.sin_port = htons(6969);

    server_size = sizeof(server);
    std::cout << "server_size " << server_size << std::endl;

    pipe = popen("ps -u root -o pid,ppid,cmd | awk '$2 == 0 || $2 == 1 || $2 == 2 {print $1}'", "r");
    ret = fread(buf, 1, sizeof(buf), pipe);

    ret = sendto(sock_id, buf, ret, 0, (struct sockaddr*)&server, server_size);
    if (ret < 0) {
        perror("Failed to send data");
        exit(1);
    }

    ret = recvfrom(sock_id, buf, sizeof(buf), 0, (struct sockaddr*)&server, &server_size);
    if (ret < 0) {
        perror("Failed to recv data");
        exit(1);
    }

    std::cout << "Server reply: " << buf << std::endl;

    close(sock_id);

    return 0;
}
