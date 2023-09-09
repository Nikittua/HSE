#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
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
    char *shared_memory_server = (char *)shmat(shm_id, NULL, 0);

    // Получение списка файлов текущего каталога
    DIR *dir;
    struct dirent *entry;
    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        exit(1);
    }

    // Очищаем разделяемую область памяти перед записью
    memset(shared_memory_server, 0, MSGSZ);

    // Записываем каждое имя файла и его время создания в разделяемую область памяти
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            struct stat file_stat;
            char file_info[256];
            if (stat(entry->d_name, &file_stat) == -1) {
                perror("stat");
                exit(1);
            }
            snprintf(file_info, sizeof(file_info), "%s Время создания: %s\n", entry->d_name, ctime(&file_stat.st_ctime));
            strcat(shared_memory_server, file_info);
        }
    }
    closedir(dir);

    // Определение идентификатора процесса, который последним отсоединялся от РОП
    struct shmid_ds shm_ds;
    if (shmctl(shm_id, IPC_STAT, &shm_ds) == -1) {
        perror("shmctl");
        exit(1);
    }

    printf("Время создания файлов:\n%s", shared_memory_server);

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
