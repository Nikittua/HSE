#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
    int sock_id, ret;
    struct sockaddr_in server;
    char buf[3000];
    unsigned server_size;
    FILE *pipe;


    sock_id = socket(AF_INET, SOCK_DGRAM, 0); //создаем дискриптор сокета домена интернета IPv4 с типом сокета 
                                           //SOCK_DGRAM. 0 ставим чтобы выбрать протокол
                                           // по умолчанию для данного домена и типа сокета   
    if (sock_id < 0) {
        perror("Failed to get sock_id");
        exit(1);
    }

    server.sin_family = AF_INET; // указывам домен
    server.sin_addr.s_addr = INADDR_ANY; // адрес IPv4 INADDR_ANY - возможность принимать пакеты с любого сетевого интерфейса, установленного в системе
    server.sin_port = htons(6969); // порт взяли из преобразования аппаратного(host) в сетевой(network) s (short)

    pipe = popen("ps -u root | awk '{print $1}' | sed '1d'", "r"); // записываем в pipe результат выполнения команды
    ret = fread(buf, 1, sizeof(buf), pipe);// возвращает количество действительно считанных объектов

    ret = sendto(sock_id, buf, ret, 0, (struct sockaddr*)&server, sizeof(server)); // отправляем серверу количество байтов ret в buf.
    if (ret < 0) {
        perror("Failed to send data");
        exit(1);
    }

    ret = recvfrom(sock_id, buf, sizeof(buf), 0, (struct sockaddr *)&server, &server_size); // получаем данные от сервера и записываем их в buf
    if (ret < 0) {
        perror("Failed to recv data");
        exit(1);
    }

    buf[ret] = 0;

    std::cout << "Server reply: " << buf << std::endl;

    return 0;
}