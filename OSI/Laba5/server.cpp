#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MSGSZ     128

struct message_buf {
    long mtype;
    char mtext[MSGSZ];
};

int main()
{
    int msqid;  // Идентификатор очереди сообщений
    int msgflg = IPC_CREAT | 0666; // Флаги для создания очереди
    key_t key; // Ключ для доступа к очереди
    struct message_buf sbuf; // Структура для сообщения
    size_t buf_length;

    key = 10;  // Устанавливаем ключ для очереди (можно использовать другой уникальный ключ)

    printf("Calling msgget with key %#x and flag %#o\n", key, msgflg);

    // Создаем или получаем доступ к очереди сообщений
    if ((msqid = msgget(key, msgflg)) < 0) {
        perror("msgget"); // Выводим сообщение об ошибке, если что-то пошло не так
        exit(1);
    } else {
        printf("msgget: msgget succeeded: msqid = %d\n", msqid);
    }

    // Открываем файл для чтения
    FILE* file = fopen("your_file.txt", "r");
    if (!file) {
        perror("fopen");
        exit(1);
    }

    // Читаем содержимое файла и отправляем его в очередь
    sbuf.mtype = 1; // Устанавливаем тип сообщения
    while (fgets(sbuf.mtext, MSGSZ, file) != NULL) {
        buf_length = strlen(sbuf.mtext) + 1;

        // Отправляем сообщение в очередь
        if (msgsnd(msqid, &sbuf, buf_length, IPC_NOWAIT) < 0) {
            perror("msgsnd");       // Выводим сообщение об ошибке, если отправка не удалась
            fclose(file);
            exit(1);
        } else {
            printf("Message send: ");
            printf(sbuf.mtext);
        }
    }

    fclose(file); // Закрываем файл
    exit(0);
}
