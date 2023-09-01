#  Лабораторная работа №5 «Обмен сообщениями между асинхронными процессами»

Вариант №21

**Сервер**. Создать очередь сообщений. Записать в качестве 1-ого сообщения имена 
всех процессов, приоритет которых больше 30, в качестве 2-го сообщения время работы 
этих процессов.

**Клиент**. Определить из информации, записанной в сообщениях очереди, общее 
количество выделенных процессов и идентификатор самого «старого» работающего 
процесса, а также время, когда из очереди было прочитано самое последнее сообщение. 
Удалить очередь сообщений.

В качестве сервера выступает файл: `server.cpp`

В качестве клиента выступает файл: `client.cpp`

# Как запустить код


```bash
# Server
g++ -o server server.cpp
./server.cpp
-------------------------

Output:
Calling msgget with key 0xa and flag 01666
msgget: msgget succeeded: msqid = 32768
Message: "Тут должно быть сообщение" Sent
```



```bash
# Client
g++ -o client client.cpp
./client.cpp
-------------------------

Ожидаемый вывод:
"Тут должно быть сообщение"
```