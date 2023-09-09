#ifndef SHARED_H
#define SHARED_H

#include <sys/ipc.h>
#include <sys/sem.h>

// Функция для создания и инициализации семафора
int createSemaphore(key_t key) {
    int sem_id = semget(key, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget");
        exit(1);
    }

    if (semctl(sem_id, 0, SETVAL, 0) == -1) {
        perror("semctl");
        exit(1);
    }

    return sem_id;
}
// Функция для ожидания клиента
void waitForClient(int sem_id) {
    struct sembuf wait_op;
    wait_op.sem_num = 0;// Индекс семафора в массиве
    wait_op.sem_op = -1;// Операция ожидания
    wait_op.sem_flg = 0;

    if (semop(sem_id, &wait_op, 1) == -1) {
        perror("semop");
        exit(1);
    }
}
// Функция для увеличения значения семафора
void signalServer(int sem_id) {
    struct sembuf signal_op;
    signal_op.sem_num = 0;// Индекс семафора в массиве
    signal_op.sem_op = 1;// Операция увеличения
    signal_op.sem_flg = 0;

    if (semop(sem_id, &signal_op, 1) == -1) {
        perror("semop");
        exit(1);
    }
}

#endif
