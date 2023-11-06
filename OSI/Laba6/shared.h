#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>



union semun {
    int val;               // значение для SETVAL
    struct semid_ds *buf;  // буфер для IPC_STAT, IPC_SET
    unsigned short *array; // массив для GETALL, SETALL
    struct seminfo *__buf; // буфер для IPC_INFO
};

// Функция для создания и инициализации семафора
int createSemaphore(key_t key) {
    int sem_id = semget(key, 1, IPC_CREAT | 0666); 
    if (sem_id == -1) {
        perror("semget");
        exit(EXIT_FAILURE);
    }


    union semun arg;
    arg.val = 0; // Установка начального значения семафора

    // Инициализация семафоров
    if (semctl(sem_id, 0, SETVAL, arg) == -1) {
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