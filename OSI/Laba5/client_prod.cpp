#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> // Для работы с временем
#include <unistd.h> // Для sleep
#include "message_utils.h" // Включаем наш заголовочный файл

int main() {
    key_t key = 10; // Устанавливаем ключ для доступа к очереди
    int msqid = -1; // Идентификатор очереди сообщений
    int timewait = 60 // время ожидания создания очереди

    printf("Ожидание создания очереди...\n");

    // Ожидание создания очереди в течение 60 секунд
    for (int i = 0; i < timewait; i++) {
        // Пытаемся получить доступ к существующей очереди сообщений
        msqid = msgget(key, 0);
        if (msqid != -1) {
            break; // Если очередь создана, выходим из цикла
        }
        sleep(1); // Ждем 1 секунду перед следующей попыткой
    }

    if (msqid == -1) {
        // Если не удалось получить доступ к очереди сообщений после 60 секунд, выходим с ошибкой
        fprintf(stderr, "Не удалось получить доступ к очереди сообщений\n");
        exit(1);
    }

    // В этой точке у вас есть доступ к очереди сообщений

    // Получаем время последнего чтения и выводим его
    time_t lastReadTime = getLastReadTime(msqid);
    printf("Время последнего чтения из очереди: %s", ctime(&lastReadTime));

    // Получаем число сообщений в очереди и выводим его
    int numMessages = getNumMessages(msqid);
    printf("Число сообщений в очереди: %d\n", numMessages);

    int oldestProcessID = getOldestProcessID(msqid);
    printf("Идентификатор самого 'старого' работающего процесса: %d\n", oldestProcessID);

    // Удаляем очередь сообщений
    if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(1);
    }

    exit(0);
}
