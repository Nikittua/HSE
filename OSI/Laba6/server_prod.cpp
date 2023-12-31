#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "shared.h"

#define MSGSZ 2048

// Функция для получения времени создания файла
time_t getFileCreationTime(const char *filename) {
    struct stat attr;
    if (stat(filename, &attr) == -1) {
        perror("stat");
        exit(1);
    }
    return attr.st_ctime;
}

//  ls -l $(file * | grep ELF | cut -f 1 -d :) | awk '{print $6, $7, $8}'

int main() {
    int shm_id; // Идентификатор разделяемой области памяти
    key_t shm_key = 10;
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
    memset(shared_memory_server, 0, MSGSZ);


    // Ожидание клиента
    printf("Ожидание клиента...\n");
    manipulateSemaphore(sem_id,-1); // Блокировка 

    struct shmid_ds shm_ds;
    if (shmctl(shm_id, IPC_STAT, &shm_ds) == -1) {
        perror("shmctl");
        exit(1);
    }

    
    // Получение имен файлов из разделяемой области памяти
    // printf("Содержимое разделяемой области памяти (РОП):\n%s\n", shared_memory_server);
    

    // Обработка имен файлов и вывод времени их создания
    char *filename = strtok(shared_memory_server, "\n");
    while (filename != 0) {
        time_t creation_time = getFileCreationTime(filename);
        printf("Имя файла: %s, Время создания: %s", filename, ctime(&creation_time));
        filename = strtok(0, "\n");
    }
    printf("\n");
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
