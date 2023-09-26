#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <time.h>
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

// Функция для получения времени создания файла
char* getFileCreationTime(const char *filename) {
    struct stat file_stat;
    if (stat(filename, &file_stat) == -1) {
        perror("stat");
        return "Ошибка при определении времени создания";
    }

    char *time_str = ctime(&file_stat.st_ctime);
    // Удаление символа новой строки из строки времени
    size_t len = strlen(time_str);
    if (len > 0 && time_str[len - 1] == '\n') {
        time_str[len - 1] = '\0';
    }

    return time_str;
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
    if ((sem_id = semget(shm_key, 0, 0)) == -1) {
        perror("semget");
        exit(1);
    }

    // Присоединение к разделяемой области памяти
    char *shared_memory = (char *)shmat(shm_id, 0, 0);

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
            char file_info[256];
            snprintf(file_info, sizeof(file_info), "%s Время создания: %s\n", entry->d_name, getFileCreationTime(entry->d_name));
            strcat(file_list, file_info);
        }
    }
    closedir(dir);

    // Копируем сформированный список файлов в разделяемую область памяти
    strncpy(shared_memory, file_list, MSGSZ);

    printf("Содержимое разделяемой области памяти (РОП):\n%s\n", shared_memory);

    // Увеличиваем значение семафора, чтобы сообщить серверу, что данные записаны
    manipulateSemaphore(sem_id, SEM_SIGNAL);

    // Отсоединение от разделяемой области памяти
    shmdt(shared_memory);

    // Завершение работы клиента

    return 0;
}
