#include <fstream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>  //Для getpid()
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
    // Вызов функции для обработки вывода ps и записи в файл
    processPSOutputAndWriteToFile();

    int msqid;  // Идентификатор очереди сообщений
    int msgflg = IPC_CREAT | 0666; // Флаг создания очереди сообщений
    key_t key; // Ключ для доступа к очереди
    struct message_buf sbuf; // Структура сообщения
    size_t buf_length;

    key = 10;  // Устанавливаем ключ для доступа к очереди

    printf("Calling msgget with key %#x and flag %#o\n", key, msgflg);

    // Создаем или получаем доступ к очереди сообщений
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
    while (fgets(sbuf.mtext, MSGSZ, file) != NULL) {
        buf_length = strlen(sbuf.mtext) + 1;

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

