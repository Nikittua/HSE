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

    // Задаем имя гнезду (привязываем его к порту)
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345); // Замените на нужный порт
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка присвоения имени гнезду");
        exit(1);
    }

    printf("Сервер готов к приему данных от клиента.\n");

    // Бесконечный цикл для обработки клиентских запросов
    while (1) {
        char buf[1024];
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);

        // Принимаем данные от клиента
        recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&client_addr, &len);

        // Определение системной составляющей приоритета
        int priority = getpriority(PRIO_PROCESS, 0);

        // Отправляем приоритет клиенту
        sendto(sockfd, &priority, sizeof(int), 0, (struct sockaddr *)&client_addr, len);
    }

    // Закрываем гнездо (обычно не достижимо из-за бесконечного цикла)
    close(sockfd);

    return 0;
}
