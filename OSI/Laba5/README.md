#  Лабораторная работа №5 «Обмен сообщениями между асинхронными процессами»

Вариант №21

**Сервер**. Создать очередь сообщений. Записать в качестве 1-ого сообщения имена 
всех процессов, приоритет которых больше 30, в качестве 2-го сообщения время работы 
этих процессов.

**Клиент**. Определить из информации, записанной в сообщениях очереди, общее 
количество выделенных процессов и идентификатор самого «старого» работающего 
процесса, а также время, когда из очереди было прочитано самое последнее сообщение. 
Удалить очередь сообщений.


# Как выполнять

```
git clone https://github.com/Nikittua/HSE.git

cd /home/user/HSE/OSI/Laba5

g++ -o client client_prod.cpp
g++ -o server server_prod.cpp
g++ -o read read_msg.cpp

./server
./client

```

# Файлы

## message_utils.h

В этом файле хранятся основные функции, а именно:
 - __receiveMessage()__ - Функция для чтения сообщения из очереди: Она просто читает сообщение из очереди. Без вывода его самого.

 - __getLastReadTime()__ - Функция для получения времени последнего чтения из очереди:  Используем msgctl с флагом IPC_STAT, чтобы получить информацию о состоянии очереди и .msg_rtime - Время последнего получения. Возвращает значение типа _time_t_

 - __getNumMessages()__ - Функция для получения числа сообщений в очереди: Так же как и в предыдущем, используем msgctl с флагом IPC_STAT, чтобы получить информацию о состоянии очереди и .msg_qnum - число сообщений в очереди. Возвращает значение типа _int_
----------------------------------------------------------------------


## server_prod.cpp

Функция __processPSOutputAndWriteToFile()__ выполняет команду: 
```
"ps -eo ni,etime,cmd", "r"
``` 
открывает файл your_file.txt и записывает туда данные в нужном формате.

Далее инифиализируются нужные поля для создания очереди и создается сама очередь с permission 666 `rw-rw-rw-`. 

Файл _your_file.txt_ открывается на чтение и данные считываются из него в очередь сообщений построчно.

Сообщения отправляются в очередь

Закрывает файл

### Выполнение
```bash
# Server
g++ -o server server.cpp
./server
-------------------------

Output:
Calling msgget with key 0xa and flag 0666
msgget: msgget succeeded: msqid = 327680
Message send: Process:khugepaged
Message send: Time:37-02:15:26

```
----------------------------------------------------------------------


## client_prod.cpp
Инифиализируются нужные поля для получения доступа к очереди с нужными правами и вызываются функции из message_utils.h

### Выполнение
```bash
# Client
g++ -o get_pid get_pid.cpp
./get_pid
-------------------------

Output:
Время последнего чтения из очереди: Sat Sep  2 14:21:58 2023
Число сообщений в очереди: 1

```
----------------------------------------------------------------------

## read_msg.cpp

Аналогично с client_prod.cpp, но выполняется лишь функция __receiveMessage()__ для чтения сообщения и дальнейший его вывод.

Будет использоваться при желании для демонстрации того, что функция __getLastReadTime()__ работает корреткно 

### Выполнение
```bash
g++ -o read_msg read_msg.cpp
./read_msg.cpp
-------------------------

Ожидаемый вывод:
Полученное сообщение: Time:36-23:24:49
```



# TODO

~~Понять как получить идентификатор самого «старого» работающего 
процесса из информации об очереди: вроде как делается с помощью __msg_lrpid__, но везде написано что __msg_lrpid__ - pid последнего процесса вызвавшего __msgrcv()__. Но считается ли он самым «старым» работающим
процессом~~ 

~~Удалить очередь сообщений: должно делаться с помощью __IPC_RMID__~~


В message.h поменять все функции чтобы они не обращались к ядру миллион раз
Книги в вариантах стиве и Раго Unix для профессионалов


## Ресурсы:
https://www.ibm.com/docs/en/aix/7.2?topic=i-ipcs-command
https://www.opennet.ru/base/dev/ipc_msg.txt.html
https://www.opennet.ru/docs/RUS/linux_base/node212.html
https://it.wikireading.ru/14109







