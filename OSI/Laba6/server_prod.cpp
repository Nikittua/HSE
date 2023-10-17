#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "shared.h"

#define MSGSZ 2048

int main() {
    int shm_id; // Идентификатор разделяемой области памяти
    key_t shm_key = 10; // Ключ для доступа к РОП
    int sem_id;

    // Создание и инициализация семафора
    sem_id = createSemaphore(shm_key);

    // Создание разделяемой области памяти
    shm_id = shmget(shm_key, MSGSZ, IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Присоединение к разделяемой области памяти
    char *shared_memory_server = (char *)shmat(shm_id, 0, 0);

    // Ожидание клиента
    printf("Ожидание клиента...\n");
    manipulateSemaphore(sem_id, SEM_SERVER, -1); // Блокировка SEM_SERVER

     struct shmid_ds shm_ds;
    if (shmctl(shm_id, IPC_STAT, &shm_ds) == -1) {
        perror("shmctl");
        exit(1);
    }

    // Вывод содержимого разделяемой области памяти (РОП)
    printf("Содержимое разделяемой области памятиs (РОП):\n%s\n", shared_memory_server);
    printf("Идентификатор процесса, последний отсоединившийся от РОП: %d\n", shm_ds.shm_lpid);


    // Отсоединение от разделяемой области памяти
    shmdt(shared_memory_server);

    // Удаление РОП
    if (shmctl(shm_id, IPC_RMID, 0) == -1) {
        perror("shmctl");
        exit(EXIT_FAILURE);
    }

    // Удаление семафора
    if (semctl(sem_id, 0, IPC_RMID) == -1) {
        perror("semctl");
        exit(EXIT_FAILURE);
    }

    return 0;
}
