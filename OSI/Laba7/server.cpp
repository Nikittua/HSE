#include <stdio.h> //стандартные функции ввода вывода
#include <stdlib.h> // функции для управления памятью
#include <unistd.h>
#include <string.h> // функции для работы со строками
#include <sys/types.h> // содержит определения различных типов данных
#include <sys/socket.h> // предоставляет функции и структуры данных для программ, использующих сокеты
#include <arpa/inet.h> // функции для преобразования IP-адресов
#include <netinet/in.h> // содержит определения структур и констант для работы с сетевыми протоколами
#include <assert.h> // для проверки кода и ошибок



unsigned proccess_pids(void *data)
{
	char *pid = strtok((char*)data, "\n"); // разделяем строки data на подстроки используя символ \n
    char system_buf[100];
	char tmp[3000];
	unsigned offset = 0;
	FILE *pipe;

	while (pid) {
    // Формируем команду для получения информации о процессе
    snprintf(system_buf, sizeof(system_buf), "ps -o nice='' -p %s", pid);
    // Открываем pipe для выполнения команды
    pipe = popen(system_buf, "r");

    // Считываем результат выполнения команды в буфер tmp
    offset += fread(tmp + offset, 1, sizeof(tmp) - offset, pipe);
    
    // Закрываем pipe
    pclose(pipe);
    assert(offset < 3000);

    // Получаем следующий PID
    pid = strtok(NULL, "\n");
	}

	memcpy(data, tmp, offset); // перезаписываем данные в data вместе со сдвигом

	return offset;
}

int main(void)
{
	int sock_id, ret;
	struct sockaddr_in addr, client;
	char buf[3000];
	unsigned client_size;

// создаем и определяем сокет
	sock_id = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_id < 0) {
		perror("Failed to create sock_id");
		exit(1);
	}

	addr.sin_family = AF_INET; // AF_INET - Домен Интернета IPv4
	addr.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY - адресам всех сетевых интерфейсов в системе
	addr.sin_port = htons(6969);  // номер порта host to network short

// ассоуциируем адрес с сокетом
	ret = bind(sock_id, (struct sockaddr *) &addr, sizeof(addr));
	if (ret) {
		perror("Failed to bind sock_id");
		exit(1);
	}

	client_size = sizeof(client);

    //std::cout << "client_size " << client_size << std::endl;

	ret = recvfrom(sock_id, buf, sizeof(buf), 0, (struct sockaddr *)&client, &client_size); // получаем данные от клиента
	if (ret <= 0) {
		perror("Failed to recv from sock_id");
		exit(1);
	}

	ret = proccess_pids(buf); // применяем к ним функцию proccess_pids

	ret = sendto(sock_id, buf, ret, 0, (struct sockaddr *)&client, sizeof(client)); // отправляем данные клиенту
	if (ret <= 0) {
		perror("Failed to send");
		exit(10);
	}
	shutdown(sock_id, SHUT_RDWR);// запрет отправки данных и диактивирует сокет

/*Если отправитель вызвал функцию shutdown (раздел 16.2), чтобы завершить
передачу данных, или если протокол поддерживает завершение передачи по
умолчанию и отправитель закрыл свой сокет, то функция recv вернет значе
ние 0 после получения всех данных.*/
}

