#include <fstream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>  //Для getpid()
#include <iostream>
#include "message_utils.h"

#define MSGSZ 128



void processPSOutputAndWriteToFile() {
    // Выполняем команду ps с опциями для получения nice value и времени работы
    FILE* pipe = popen("ps -eo ni,etime,cmd", "r");
    if (!pipe) {
        std::cerr << "Ошибка при выполнении команды ps." << std::endl;
        return;
    }

    char buffer[128];
    std::ofstream outfile("your_file.txt"); // Открываем файл для записи

    while (fgets(buffer, sizeof(buffer), pipe) != 0) { // считываем из pipe и помещаем в buffer
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
    // Вызов функции для обработки вывода ps и записи в файл
    processPSOutputAndWriteToFile();

    int msqid;  // Идентификатор очереди сообщений
    int msgflg = IPC_CREAT | 0666; // Флаг создания очереди сообщений
    key_t key; // идентификатор для доступа к очереди
    struct message_buf sbuf; // Структура сообщения для помещения его в очередь
    size_t buf_length;

    key = 10;  // Устанавливаем ключ для доступа к очереди
 
    printf("Calling msgget with key %#x and flag %#o\n", key, msgflg);

    // Создаем очередь сообщений
    if ((msqid = msgget(key, msgflg)) < 0) {
        perror("msgget");
        exit(1);
    } else {
        printf("msgget: msgget succeeded: msqid = %d\n", msqid);
    }

    //Открываем файл для чтения
    FILE* file = fopen("your_file.txt", "r");
    if (!file) {
        perror("fopen");
        exit(1);
    }

    // Читаем содержимое файла и отправляем его в очередь сообщений
    sbuf.mtype = 1; // Устанавливаем тип сообщения
    while (fgets(sbuf.mtext, MSGSZ, file) != 0) { // считываем из file и помещаем в mtext в структуре сообщеня
        buf_length = strlen(sbuf.mtext);

        // Отправляем сообщение в очередь сообщений
        if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) {
            perror("msgsnd");
            fclose(file);
            exit(1);
        } else {
            printf("Message send: %s", sbuf.mtext);
        }
    }

    fclose(file); // Закрываем файл
    exit(0);
}

