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

// Function to check if a file is an ELF executable using popen
void listELFExecutablesWithTime(char *output) {
    FILE *fp = popen("file * | grep ELF | cut -f 1 -d :", "r");
    if (fp == NULL) {
        perror("popen");
        exit(1);
    }

    char result[256];
    sprintf(output, "ELF Executables:\n");

    while (fgets(result, sizeof(result), fp) != NULL) {
        // Remove trailing newline character
        size_t len = strlen(result);
        if (len > 0 && result[len - 1] == '\n') {
            result[len - 1] = '\0';
        }

        // Get file creation time
        struct stat file_stat;
        if (stat(result, &file_stat) == -1) {
            perror("stat");
            exit(1);
        }

        sprintf(output + strlen(output), "%s (Created at: %s)\n", result, ctime(&file_stat.st_ctime));
    }

    pclose(fp);
}

int main() {
    int shm_id;
    key_t shm_key = 10;
    int sem_id;

    shm_id = shmget(shm_key, MSGSZ, IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget");
        exit(1);
    }

    if ((sem_id = semget(shm_key, 2, 0)) == -1) {
        perror("semget");
        exit(1);
    }

    char *shared_memory = (char *)shmat(shm_id, 0, 0);

    listELFExecutablesWithTime(shared_memory);

    // Разблокировать сервер и заблокировать себя
    manipulateSemaphore(sem_id, SEM_SERVER, 1); // разблокировка сервера
    // manipulateSemaphore(sem_id, SEM_CLIENT, -1);  // блокировка себя
    shmdt(shared_memory);


    return 0;
}