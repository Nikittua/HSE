#ifndef MESSAGE_UTILS_H
#define MESSAGE_UTILS_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MSGSZ 128


// Структура для использования при чтении сообщения
struct my_msgbuf {
    long mtype;
    char mtext[MSGSZ];
};
// Структура для создания сообщения и помещения его в очередь
struct message_buf {
    long mtype;
    char mtext[MSGSZ];
};


// Функция для чтения сообщения из очереди
void receiveMessage(int msqid, struct my_msgbuf *rbuf) {
    // Получаем сообщение из очереди с типом 1
    if (msgrcv(msqid, rbuf, MSGSZ, 1, 0) < 0) {
        perror("msgrcv"); // Выводим сообщение об ошибке, если не удается получить сообщение
        exit(1);
    }
}

// Функция для получения времени последнего чтения из очереди
time_t getLastReadTime(int msqid) {
    struct msqid_ds buf; // Структура для получения информации о состоянии очереди

    // Используем msgctl с флагом IPC_STAT, чтобы получить информацию о состоянии очереди
    if (msgctl(msqid, IPC_STAT, &buf) == -1) {
        perror("msgctl");
        exit(1);
    }

    return buf.msg_rtime; // Возвращаем время последнего чтения
}

// Функция для получения числа сообщений в очереди
int getNumMessages(int msqid) {
    struct msqid_ds buf; // Структура для получения информации о состоянии очереди

    // Используем msgctl с флагом IPC_STAT, чтобы получить информацию о состоянии очереди
    if (msgctl(msqid, IPC_STAT, &buf) == -1) {
        perror("msgctl");
        exit(1);
    }

    return buf.msg_qnum; // Возвращаем число сообщений в очереди
}

// Функция для получения самого старого процесса
int getOldestProcessID(int msqid) {
    struct msqid_ds buf; // Структура для получения информации о состоянии очереди

    // Используем msgctl с флагом IPC_STAT, чтобы получить информацию о состоянии очереди
    if (msgctl(msqid, IPC_STAT, &buf) == -1) {
        perror("msgctl");
        exit(1);
    }

    return buf.msg_lrpid; // Возвращаем идентификатор последнего процесса, который читал из очереди
}


#endif

