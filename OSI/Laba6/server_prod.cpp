#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/sem.h>
#include <time.h> // Для работы с временем
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

    // Ожидание клиента
    printf("Ожидание клиента...\n");
    waitForClient(sem_id);

    // Присоединение к разделяемой области памяти
    //Объявляет указатель на символьный массив и инициализирует его значением
    char *shared_memory_server = (char *)shmat(shm_id, NULL, 0);

    // Определение времени создания файлов
    struct stat dir_stat;
    if (stat(".", &dir_stat) == -1) {
        perror("stat");
        exit(1);
    }

    // Определение идентификатора процесса, который последним отсоединялся от РОП
    struct shmid_ds shm_ds;
    if (shmctl(shm_id, IPC_STAT, &shm_ds) == -1) {
        perror("shmctl");
        exit(1);
    }

    printf("Время создания файлов: %s", ctime(&dir_stat.st_ctime));
    
    printf("Идентификатор процесса, последний отсоединившийся от РОП: %d\n", shm_ds.shm_lpid);

    // Отсоединение от разделяемой области памяти
    shmdt(shared_memory_server);

    // Удаление РОП
    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
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
