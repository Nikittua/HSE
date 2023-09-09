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

#include "shared.h"
#define MSGSZ 128

int main() {
    int shm_id; // Идентификатор разделяемой области памяти
    key_t shm_key = 10; // Ключ для доступа к РОП
    int sem_id;

    // Получение доступа к существующей разделяемой области памяти
    shm_id = shmget(shm_key, MSGSZ, 0666);
    if (shm_id == -1) {
        perror("shmget");
        exit(1);
    }

    // Получение доступа к существующему семафору
    if ((sem_id = semget(shm_key, 1, 0666)) == -1) {
        perror("semget");
        exit(1);
    }

    // Присоединение к разделяемой области памяти
    char *shared_memory = (char *)shmat(shm_id, NULL, 0);

    // Получение списка файлов текущего каталога
    DIR *dir;
    struct dirent *entry;
    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        exit(1);
    }

    // Собираем и формируем список файлов в строку
    char file_list[MSGSZ] = "";
    while ((entry = readdir(dir)) != NULL) {
        strcat(file_list, entry->d_name);
        strcat(file_list, "\n");
    }
    closedir(dir);

    // Копируем сформированный список файлов в разделяемую область памяти
    strncpy(shared_memory, file_list, MSGSZ);

    printf("Содержимое разделяемой области памяти (РОП):\n%s\n", shared_memory);

    // Увеличиваем значение семафора, чтобы сообщить серверу, что данные записаны
    signalServer(sem_id);

    // Отсоединение от разделяемой области памяти
    shmdt(shared_memory);

    // Завершение работы клиента

    return 0;
}
