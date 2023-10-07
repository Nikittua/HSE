#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <sys/sem.h>
#include "shared.h"

#define MSGSZ 128

// Function to check if a file is an ELF executable using popen
bool isELFExecutable(const char *filename) {
    char command[100];
    snprintf(command, sizeof(command), "file \"%s\" | grep ELF | cut -f 1 -d :", filename);

    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        perror("popen");
        return false;
    }

    char result[256];
    if (fgets(result, sizeof(result), fp) != NULL) {
        if (strstr(result, filename) != NULL) {
            pclose(fp);
            return true;
        }
    }

    pclose(fp);
    return false;
}

// Function to get the file creation time
char *getFileCreationTime(const char *filename) {
    struct stat file_stat;
    if (stat(filename, &file_stat) == -1) {
        perror("stat");
        return "Error determining creation time";
    }

    char *time_str = ctime(&file_stat.st_ctime);
    size_t len = strlen(time_str);
    if (len > 0 && time_str[len - 1] == '\n') {
        time_str[len - 1] = '\0';
    }

    return time_str;
}

int main() {
    int shm_id;
    key_t shm_key = 10;
    int sem_id;

    shm_id = shmget(shm_key, MSGSZ, 0);
    if (shm_id == -1) {
        perror("shmget");
        exit(1);
    }

    if ((sem_id = semget(shm_key, 0, 0)) == -1) {
        perror("semget");
        exit(1);
    }

    char *shared_memory = (char *)shmat(shm_id, 0, 0);

    DIR *dir;
    struct dirent *entry;
    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        exit(1);
    }

    char file_list[MSGSZ] = "";
    while ((entry = readdir(dir)) != NULL) {
        if (isELFExecutable(entry->d_name)) {
            char file_info[256];
            snprintf(file_info, sizeof(file_info), "%s Creation Time: %s\n", entry->d_name, getFileCreationTime(entry->d_name));
            strcat(file_list, file_info);
        }
    }
    closedir(dir);

    strncpy(shared_memory, file_list, MSGSZ);

    printf("Shared Memory Content:\n%s\n", shared_memory);

    // Assuming you have a manipulateSemaphore function
    manipulateSemaphore(sem_id, SEM_SIGNAL);

    shmdt(shared_memory);

    return 0;
}
