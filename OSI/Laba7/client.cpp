#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <dirent.h>
#include <ctype.h>

int main() {
    int sock;
    struct sockaddr_in addr;
    char buf[4096];

    // Определение идентификаторов системных процессов
    DIR *dir;
    struct dirent *entry;

    dir = opendir("/proc");
    if (dir == NULL) {
        perror("Ошибка открытия каталога /proc");
        exit(1);
    }

    memset(buf, 0, sizeof(buf));

    while ((entry = readdir(dir)) != NULL) {
        if (isdigit(entry->d_name[0])) {
            int pid = atoi(entry->d_name);
            if (pid <= 1000) {
                snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), "PID: %d\n", pid);
            }
        }
    }

    closedir(dir);

    // Создаем гнездо
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(2);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425); 
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); 

    // Устанавливаем соединение с сервером
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect");
        exit(3);
    }

    // Отправляем данные на сервер
    send(sock, buf, strlen(buf), 0);

    // Получаем ответ от сервера
    recv(sock, buf, sizeof(buf), 0);

    printf("Ответ от сервера:\n%s\n", buf);

    // Закрываем гнездо
    close(sock);

    return 0;
}

