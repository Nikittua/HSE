#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

int main() {
    int shm_id; // Идентификатор разделяемой области памяти
    int sem_id; // Идентификатор набора семафоров
    key_t shm_key = 10; // Генерируем ключ для РОП
    key_t sem_key = 10; // Генерируем ключ для НС

    // Создание разделяемой области памяти
    shm_id = shmget(shm_key, sizeof(int), IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget");
        exit(1);
    }

    // Создание набора семафоров (предположим, что нужен только один семафор)
    sem_id = semget(sem_key, 1, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget");
        exit(1);
    }

    printf("Разделяемая область памяти и набор семафоров созданы.\n");
    

    return 0;
}
