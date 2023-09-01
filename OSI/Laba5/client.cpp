#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>

#define MSGSZ     128

struct msgbuf {
    long mtype;
    char mtext[MSGSZ];
} message_buf;

main()
{
    int msqid;                  // Идентификатор очереди сообщений
    key_t key;                   // Ключ для доступа к очереди
    struct msgbuf rbuf;          // Структура для приема сообщения

    key = 10;                    // Устанавливаем ключ для доступа к очереди

    // Получаем доступ к существующей очереди сообщений
    if ((msqid = msgget(key, 0666)) < 0) {
        perror("msgget");        // Выводим сообщение об ошибке, если не удается получить доступ к очереди
        exit(1);
    }

    // Получаем сообщение из очереди с типом 1
    if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0) {
        perror("msgrcv");        // Выводим сообщение об ошибке, если не удается получить сообщение
        exit(1);
    }

    printf("%s\n", rbuf.mtext); // Выводим текст полученного сообщения
    exit(0);
}
