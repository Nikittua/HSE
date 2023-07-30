#include <iostream>
#include <string>
#include <vector>
#include <cassert>


// Класс для представления события
class Event {
private:
    std::string timestamp;
    std::string severity;
    std::string message;

public:
    Event(const std::string& ts, const std::string& sev, const std::string& msg)
        : timestamp(ts), severity(sev), message(msg) {}

    void print() const {
        std::cout << "[" << timestamp << "] " << severity << ": " << message << std::endl;
    }
};

// Класс системы протоколирования
class Logger {
private:
    std::vector<Event> events;
    static Logger* instance;

    Logger() {}  // Приватный конструктор, чтобы предотвратить создание объектов извне

public:
    static Logger* getInstance() {
        if (instance == nullptr) {
            instance = new Logger();
        }
        return instance;
    }

    void logEvent(const std::string& severity, const std::string& message) {
        // Создаем новое событие и добавляем его в список событий
        Event event(getCurrentTimestamp(), severity, message);
        events.push_back(event);

        // Если число событий превышает 10, удаляем самое старое событие
        if (events.size() > 10) {
            events.erase(events.begin());
        }
    }

    void printLastEvents() const {
        std::cout << "Last 10 events:" << std::endl;
        for (const Event& event : events) {
            event.print();
        }
    }

    std::string getCurrentTimestamp() const {
        // Вернуть текущее время в формате, удобном для вывода
        // Здесь можно использовать любую библиотеку для работы с датой и временем
        // В данном примере просто возвращается строка "HH:MM:SS"
        return "HH:MM:SS";
    }
};

Logger* Logger::instance = nullptr;
