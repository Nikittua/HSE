#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>


// Объединение для его дальнешйего использования в semctl
union semun {
    int val;               // значение для SETVAL
    struct semid_ds *buf;  // буфер для IPC_STAT, IPC_SET
    unsigned short *array; // массив для GETALL, SETALL
    struct seminfo *__buf; // буфер для IPC_INFO
};

// Функция для создания и инициализации семафора
int createSemaphore(key_t key) {
    int sem_id = semget(key, 1, IPC_CREAT | 0666); // Создаем 1 семафор с ключём key и флагом IPC_CREATE
    if (sem_id == -1) {
        perror("semget");
        exit(1);
    }


    union semun arg;
    arg.val = 1; // Установка начального значения семафора

    // Инициализация семафоров
    if (semctl(sem_id, 0, SETVAL, arg) == -1) { // с помощью SETVAL устанавливаем значение семафора с номером 0(так как счетчик идет от 0)
        perror("semctl");
        exit(1);
    }

    return sem_id;
}


// Функция для работы с семафором
void manipulateSemaphore(int sem_id, int op) {
    // тут мы определяем с каким семафором мы будем производить операции и что за операция это будет
    struct sembuf semaphore;
    semaphore.sem_num = 0; // Номер семафора, с которым мы работаем
    semaphore.sem_op = op;       // Операция: -1 для блокировки, 1 для разблокировки

    if (semop(sem_id, &semaphore, 1) == -1) { // Функция для нескольких операций с набором семафоров. В нашем случае будет 1 операция, которая определена в структуре semaphore
        perror("semop");
        exit(1);
    }
}