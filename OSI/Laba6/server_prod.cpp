#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include "shared.h"

#define MSGSZ 128

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
        exit(1);
    }

    // Присоединение к разделяемой области памяти
    char *shared_memory_server = (char *)shmat(shm_id, 0, 0);

    // Ожидание клиента
    printf("Ожидание клиента...\n");
    manipulateSemaphore(sem_id, SEM_WAIT);


    struct shmid_ds shm_ds;
    if (shmctl(shm_id, IPC_STAT, &shm_ds) == -1) {
        perror("shmctl");
        exit(1);
    }



    // Вывод содержимого разделяемой области памяти (РОП)
    printf("Содержимое разделяемой области памяти (РОП):\n%s\n", shared_memory_server);

    printf("Идентификатор процесса, последний отсоединившийся от РОП: %d\n", shm_ds.shm_lpid);

    // Отсоединение от разделяемой области памяти
    shmdt(shared_memory_server);

    // Удаление РОП
    if (shmctl(shm_id, IPC_RMID, 0) == -1) {
        perror("shmctl");
        exit(1);
    }

    // Удаление семафора
    if (semctl(sem_id, 0, IPC_RMID) == -1) {
        perror("semctl");
        exit(1);
    }

    return 0;
}
