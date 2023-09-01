#include <iostream>
#include <cstdio>
#include <cstdlib>

int main() {
    // Выполняем команду ps с опциями для получения PID, nice value и команды
    FILE* pipe = popen("ps -eo pid,ni,cmd", "r");
    if (!pipe) {
        std::cerr << "Ошибка при выполнении команды ps." << std::endl;
        return 1;
    }

    // Парсим вывод команды
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        int pid, niceValue;
        char cmd[128];

        // Используем sscanf для извлечения PID, nice value и команды
        if (sscanf(buffer, "%d %d %[^\n]", &pid, &niceValue, cmd) == 3) {
            if (niceValue > 30) {
                std::cout << "PR = 20 + Nice Value: " << 20 + niceValue << ", Command: " << cmd << std::endl;;
            }
        }
    }

    pclose(pipe);
    return 0;
}
