from Flight import *
from Flight_generator import *
import numpy as np
import os
import matplotlib.pyplot as plt
def plot_sort_time_vs_size(algorithms, flights):
    """
    Строит график зависимости времени сортировки различных алгоритмов от размера данных.

    **Аргументы:**
        algorithms (List): список алгоритмов сортировки, которые необходимо построить.
        flights (List): список объектов Flight для сортировки.
    """
    
    size = np.arange(1, len(flights) + 1)
    sort_times = []
    labels = []
    for algorithm in algorithms:
        sort_time = np.zeros(len(flights))
        for i in range(len(flights)):
            sub_flights = flights[:i+1]
            sort_time[i] = measure_sort_time(algorithm, sub_flights)
        sort_times.append(sort_time)
        labels.append(algorithm.__name__)

    plt.figure(figsize=(8, 6))
    for i in range(len(algorithms)):
        plt.plot(size, sort_times[i], label=labels[i])
    plt.legend()
    plt.xlabel('Size of array')
    plt.ylabel('Sorting time')
    plt.savefig(os.path.join('C:/Users/nikittua/Desktop/MP1/docs/images', 'sort_time_vs_size.png'))
def save_flights_to_file(flights, file_name):
    """
    Сохраняет список объектов Flight в файл.

    **Параметры:**        
        - flights (List): список объектов полета.
        - file_name (Str): имя файла, в который будут сохраняться полеты.

    """
    with open(file_name, 'w') as f:
        for flight in flights:
            f.write(str(flight) + '\n')         
def load_flights_from_file(file_name):
    """
    Загружает объекты Flight из файла в список.

    **Параметры**
        file_name (str): Имя файла, содержащего рейсы.

    **Возвращает:**
        list: Список объектов Flight.
    """
    flights = []
    with open(file_name, 'r') as f:
        for line in f:
            flight_number, airline, arrival_date, arrival_time, passengers = line.strip().split(', ')
            flights.append(Flight(flight_number, airline, arrival_date, arrival_time, int(passengers)))
    return flights
def measure_multiple_sort_times(data):
    """
    Измеряет время, необходимое нескольким функциям сортировки для сортировки заданного массива данных.

    **Параметры:**
        data (List): данные для сортировки.

    **Возвращает:**
        Dict: словарь, содержащий время, которое потребовалось каждой функции сортировки для сортировки данных в миллисекундах.
        
    **Примечания:**
        Эта функция использует функцию `measure_sort_time` для измерения времени, 
        которое требуется следующим трем функциям сортировки для сортировки заданных данных:

        - quicksort_flights

        - bubble_sort_flights

        - Shaker_sort_flights

        Функция возвращает словарь, где ключи — это имена функций сортировки, 
        а значения — это время, которое потребовалось каждой функции для сортировки данных в миллисекундах.
    """
    times = {
        'quicksort_flights': measure_sort_time(quicksort_flights, data) * 1000
    }
    times['bubble_sort_flights'] = measure_sort_time(bubble_sort_flights, data)*1000
    times['shaker_sort_flights'] = measure_sort_time(shaker_sort_flights, data)*1000
    return times
def measure_sort_time(sort_function, data):
    """
    Измеряет время, которое требуется заданной функции для сортировки заданного массива данных.

    **Параметры:**
        sort_function (function): тестируемая функция сортировки.
        data (List): данные для сортировки.

    **Возвращает:**
        Lloat: время, которое потребовалось функции для сортировки данных

    """
    start = time.time()
    sort_function(data)
    end = time.time()
    return end - start


if __name__ == '__main__':
    num_flights = 2000
    generate_random_flights(num_flights)
    flights = load_flights_from_file('C:/Users/nikittua/Desktop/MP1/Files/flights.txt')
    times = measure_multiple_sort_times(flights)
    for sort_algorithm, sort_time in times.items():
        print(f"{sort_algorithm}: {sort_time} ms")
    save_flights_to_file(flights,'C:/Users/nikittua/Desktop/MP1/Files/sorted_flights.txt')
    algorithms = [quicksort_flights,bubble_sort_flights,shaker_sort_flights]
    plot_sort_time_vs_size(algorithms, flights)

#TEST
# flight1 = Flight("DL1234", "Delta", "2023-03-10", "15:30", 120)
# flight2 = Flight("UA5678", "Delta", "2023-03-10", "10:45", 180)
# flight3 = Flight("AA4321", "Delta", "2023-03-10", "19:15", 90)
# flights = [flight1,flight2,flight3]

# sorted_flights = quicksort_flights(flights)

# for flight in sorted_flights:
#     print(f'{flight.airline} flight arrives on {flight.arrival_date} at {flight.arrival_time} with {flight.passengers} passengers.')

