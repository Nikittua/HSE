#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> // Для работы с временем
#include "message_utils.h" // Включаем наш заголовочный файл

int main() {
    int msqid;                  // Идентификатор очереди сообщений
    key_t key;                   // Ключ для доступа к очереди

    key = 10;                    // Устанавливаем ключ для доступа к очереди

    // Получаем доступ к существующей очереди сообщений
    if ((msqid = msgget(key, 0666)) < 0) {
        perror("msgget");        // Выводим сообщение об ошибке, если не удается получить доступ к очереди
        exit(1);
    }

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

