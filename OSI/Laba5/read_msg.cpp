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
    struct my_msgbuf rbuf;       // Структура для получения сообщения

    key = 10;                    // Устанавливаем ключ для доступа к очереди

    // Получаем доступ к существующей очереди сообщений
    if ((msqid = msgget(key, 0)) < 0) {
        perror("msgget");        // Выводим сообщение об ошибке, если не удается получить доступ к очереди
        exit(1);
    }

    receiveMessage(msqid, &rbuf);
    printf("Полученное сообщение: %s\n", rbuf.mtext);
    
    exit(0);
}

