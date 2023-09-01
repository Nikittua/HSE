#include <fstream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // Add this for getpid()
#include <iostream>


#define MSGSZ 128

struct message_buf {
    long mtype;
    char mtext[MSGSZ];
};

void processPSOutputAndWriteToFile() {
    // Выполняем команду ps с опциями для получения nice value и времени работы
    FILE* pipe = popen("ps -eo ni,etime,cmd", "r");
    if (!pipe) {
        std::cerr << "Ошибка при выполнении команды ps." << std::endl;
        return;
    }

    // Парсим вывод команды
    char buffer[128];
    std::ofstream outfile("your_file.txt"); // Открываем файл для записи

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        int niceValue;
        char etime[128];
        char cmd[128];
        // Используем sscanf для извлечения nice value и времени работы
        if (sscanf(buffer, "%d %s [%[^]]]", &niceValue, etime, cmd) == 3) {
            if (niceValue > 10) {
                // Записываем nice value и время работы процессов с niceValue > 10 в файл
                outfile << "Process:" << cmd << "\n" << "Time:" << etime << std::endl;
                printf("File was changed \n");
            }
        }
    }
    // Закрываем файл и pipe
    outfile.close();
    pclose(pipe);
}

int main() {
    // Call the function to process PS output and write to the file
    processPSOutputAndWriteToFile();

    int msqid;  // Message queue identifier
    int msgflg = IPC_CREAT | 0666; // Flags for creating the message queue
    key_t key; // Key for accessing the message queue
    struct message_buf sbuf; // Structure for the message
    size_t buf_length;

    key = 10;  // Set the key for the message queue (you can use a different unique key)

    printf("Calling msgget with key %#x and flag %#o\n", key, msgflg);

    // Create or get access to the message queue
    if ((msqid = msgget(key, msgflg)) < 0) {
        perror("msgget");
        exit(1);
    } else {
        printf("msgget: msgget succeeded: msqid = %d\n", msqid);
    }

    // Open the file for reading
    FILE* file = fopen("your_file.txt", "r");
    if (!file) {
        perror("fopen");
        exit(1);
    }

    // Read the file contents and send them to the message queue
    sbuf.mtype = 1; // Set the message type
    while (fgets(sbuf.mtext, MSGSZ, file) != NULL) {
        buf_length = strlen(sbuf.mtext) + 1;

        // Send the message to the message queue
        if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) {
            perror("msgsnd");
            fclose(file);
            exit(1);
        } else {
            printf("Message send: %s", sbuf.mtext);
        }
    }

    fclose(file); // Close the file
    exit(0);
}

