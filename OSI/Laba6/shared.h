#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

// Константы для индекса семафора и операций
#define SEM_INDEX 0
#define SEM_WAIT -1
#define SEM_SIGNAL 1

// Функция для создания и инициализации семафора
int createSemaphore(key_t key) {
    int sem_id = semget(key, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        // Возвращаем код ошибки
        return -1;
    }

    if (semctl(sem_id, SEM_INDEX, SETVAL, 0) == -1) {
        // Возвращаем код ошибки
        return -1;
    }

    return sem_id;
}

// Функция для работы с семафором
int manipulateSemaphore(int sem_id, int action) {
    struct sembuf sem_op;
    sem_op.sem_num = SEM_INDEX;
    sem_op.sem_flg = 0;

    if (action == SEM_WAIT) {
        sem_op.sem_op = SEM_WAIT;
    } else if (action == SEM_SIGNAL) {
        sem_op.sem_op = SEM_SIGNAL;
    } else {
        return -1;
    }

    if (semop(sem_id, &sem_op, 1) == -1) {
        return errno;
    }

    return 0; // Успешное выполнение
}

