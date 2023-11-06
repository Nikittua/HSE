#include <iostream>
#include <cstdlib>
#include <sys/stat.h>
#include <cstring>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <dirent.h>
#include <unistd.h>
#include "shared.h"

#define MSGSZ 2048  // Увеличим размер, чтобы уместить больше информации


void listELFExecutables(char *output) {
    FILE *fp = popen("file * | grep ELF | cut -f 1 -d :", "r");
    if (fp == NULL) {
        perror("popen");
        exit(1);
    }

    char result[256];

    while (fgets(result, sizeof(result), fp) != NULL) {
        size_t len = strlen(result);
        if (len > 0 && result[len - 1] == '\n') {
            result[len - 1] = '\0';
        }

        sprintf(output + strlen(output), "%s\n", result);
    }

    // Print the result after reading from the pipe
    printf("ELF Executables:\n%s", output);

    pclose(fp);
}

int main() {
    int shm_id;
    key_t shm_key = 10;
    int sem_id;

     int timewait = 60; // время ожидания создания РОП



        printf("Ожидание создания РОП...\n");

    // Ожидание создания очереди в течение 60 секунд
    for (int i = 0; i < timewait; i++) {
        // Пытаемся получить доступ к существующей РОП
        shm_id = shmget(shm_key, 0, 0);
        if (shm_id != -1) {
            break; // Если РОП, выходим из цикла
        }
        
        sleep(1); // Ждем 1 секунду перед следующей попыткой
    }

    if (shm_id == -1) {
        // Если не удалось получить доступ к РОП после 60 секунд, выходим с ошибкой
        fprintf(stderr, "Не удалось получить доступ к РОП\n");
        exit(1);
    }


    if ((sem_id = semget(shm_key, 0, 0)) == -1) {
        perror("semget");
        exit(1);
    }

    char *shared_memory = (char *)shmat(shm_id, 0, 0);

    listELFExecutables(shared_memory);

    // Разблокировать сервер и заблокировать себя
    manipulateSemaphore(sem_id, 0, 1); // разблокировка сервера
    // manipulateSemaphore(sem_id, SEM_CLIENT, -1);  // блокировка себя
    shmdt(shared_memory);

    return 0;
}
