#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

int main() {
    // Создаем гнездо в домене INET
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Ошибка создания гнезда");
        exit(1);
    }

    // Задаем имя гнезду (необязательно в клиентской части)
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345); // Замените на порт сервера
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Замените на IP-адрес сервера

    // Определение идентификаторов системных процессов
    pid_t pid = getpid();
    pid_t ppid = getppid();

    // Отправляем идентификаторы на сервер
    sendto(sockfd, &pid, sizeof(pid_t), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    sendto(sockfd, &ppid, sizeof(pid_t), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Получаем ответ от сервера
    int priority;
    socklen_t len = sizeof(server_addr);
    recvfrom(sockfd, &priority, sizeof(int), 0, (struct sockaddr *)&server_addr, &len);

    printf("Получен приоритет от сервера: %d\n", priority);

    // Закрываем гнездо
    close(sockfd);

    return 0;
}
