#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream> // Добавим для работы с файлами

int main() {
    // Выполняем команду ps с опциями для получения nice value и времени работы
    FILE* pipe = popen("ps -eo ni,etime", "r");
    if (!pipe) {
        std::cerr << "Ошибка при выполнении команды ps." << std::endl;
        return 1;
    }

    // Парсим вывод команды
    char buffer[128];
    std::ofstream outfile("your_file.txt"); // Открываем файл для записи

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        int niceValue;
        char etime[128];
        // Используем sscanf для извлечения nice value и времени работы
        if (sscanf(buffer, "%d %s", &niceValue, etime) == 2) {
            if (niceValue > 10) {
                // Записываем nice value и время работы процессов с niceValue > 30 в файл
                outfile << "Nice: " << niceValue << "\n" << "Time: " << etime << std::endl;
            }
        }
    }

    // Закрываем файл и pipe
    outfile.close();
    pclose(pipe);

    return 0;
}
