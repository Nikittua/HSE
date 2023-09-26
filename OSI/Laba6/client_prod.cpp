#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

#include "shared.h"
#define MSGSZ 128

// Функция для проверки, является ли файл исполняемым ELF-файлом
bool isELFExecutable(const char *filename) {
    struct stat file_stat;
    if (stat(filename, &file_stat) == -1) {
        perror("stat");
        return false;
    }

    if (S_ISREG(file_stat.st_mode) && (file_stat.st_mode & S_IXUSR)) {
        int fd = open(filename, O_RDONLY);
        if (fd == -1) {
            perror("open");
            return false;
        }

        unsigned char magic;
        if (read(fd, &magic, 1) == 1 && magic == 0x7f) {
            close(fd);
            return true;
        }

        close(fd);
    }

    return false;
}

int main() {
    int shm_id; // Идентификатор разделяемой области памяти
    key_t shm_key = 10; // Ключ для доступа к РОП
    int sem_id;

    // Получение доступа к существующей разделяемой области памяти
    shm_id = shmget(shm_key, MSGSZ, 0);
    if (shm_id == -1) {
        perror("shmget");
        exit(1);
    }

    // Получение доступа к существующему семафору
    if ((sem_id = semget(shm_key, 1, 0)) == -1) {
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

    // Собираем и формируем список исполняемых ELF-файлов в строку
    char file_list[MSGSZ] = "";
    while ((entry = readdir(dir)) != NULL) {
        if (isELFExecutable(entry->d_name)) {
            strcat(file_list, entry->d_name);
            strcat(file_list, "\n");
        }
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
