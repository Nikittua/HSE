#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

// Константы для индекса семафора и операций
#define SEM_SERVER 0
#define SEM_CLIENT 1

// Функция для создания и инициализации семафора
int createSemaphore(key_t key) {
    int sem_id = semget(key, 2, IPC_CREAT | 0666); // Изменено с 1 на 2, так как у вас два семафора
    if (sem_id == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }

    // Инициализация семафоров
    if (semctl(sem_id, SEM_SERVER, SETVAL, 0) == -1 || semctl(sem_id, SEM_CLIENT, SETVAL, 0) == -1) {
        perror("semctl");
        exit(EXIT_FAILURE);
    }

    return sem_id;
}

// Функция для работы с семафором
void manipulateSemaphore(int sem_id, int sem_num, int op) {
    struct sembuf semaphore;
    semaphore.sem_num = sem_num; // Номер семафора, с которым мы работаем
    semaphore.sem_op = op;       // Операция: -1 для блокировки, 1 для разблокировки
    semaphore.sem_flg = 0;       // Флаги (обычно 0)

    if (semop(sem_id, &semaphore, 1) == -1) {
        perror("semop");
        exit(EXIT_FAILURE);
    }
}
