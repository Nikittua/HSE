def compare_flights(flight1, flight2):
    """
    .. code-block:: python

     1  if flight1.arrival_date != flight2.arrival_date:
            return (flight1.arrival_date < flight2.arrival_date) * 2 - 1 

     2  elif flight1.arrival_time != flight2.arrival_time:
            return (flight1.arrival_time < flight2.arrival_time) * 2 - 1

     3  elif flight1.airline != flight2.airline:
            return (flight1.airline < flight2.airline) * 2 - 1
            
     4  else:
            return (flight2.passengers - flight1.passengers)


    **Параметры:**
    flight1 (Flight)
    flight2 (Flight)

    **Возвращает:**

    1. Если даты прибытия не совпадают, функция возвращает отрицательное целое число, если рейс1 прибывает раньше рейса2, 
    или положительное целое число, если рейс1 прибывает позже рейса2. 
    Выражение (flight1.arrival_date < Flight2.arrival_date) * 2 - 1 используется для преобразования логического результата сравнения в целое число.

    2. Если времена прибытия не равны, возвращается отрицательное целое число, 
    если рейс1 прибывает раньше рейса2, или положительное целое число, если рейс1 прибывает позже рейса2.

    3. Если авиакомпании не равны, возвращается отрицательное целое число, 
    если название авиакомпании рейса1 стоит перед названием авиакомпании рейса2 в лексикографическом порядке, 
    или положительное целое число, если название авиакомпании рейса1 идет после названия авиакомпании рейса2.

    4. Если ни одно из вышеперечисленных условий не выполняется, возвращается разница в количестве пассажиров на двух рейсах, которая будет положительным целым числом, 
    если у рейса2 больше пассажиров, чем у рейса1, или отрицательным целым числом, 
    если у рейса2 меньше пассажиров, чем у рейса1.

    

    """
    if flight1.arrival_date != flight2.arrival_date:
        return (flight1.arrival_date < flight2.arrival_date) * 2 - 1 
    elif flight1.arrival_time != flight2.arrival_time:
        return (flight1.arrival_time < flight2.arrival_time) * 2 - 1
    elif flight1.airline != flight2.airline:
        return (flight1.airline < flight2.airline) * 2 - 1
    else:
        return (flight2.passengers - flight1.passengers)
def quicksort_flights(flights):

    """

    **Quicksort Algorithm:**
    Алгоритм быстрой сортировки начинается с выбора центрального элемента из списка рейсов,
    который используется для разделения списка на два меньших списка: один с элементами, меньшими, чем центральный, и другой, с элементами, большими, чем центральный.
    Затем алгоритм рекурсивно сортирует эти меньшие списки до тех пор, пока не будет отсортирован весь список.


    В среднем быстрая сортировка имеет сложность O(n log n), где n — количество элементов в списке.
    Однако в худшем случае он может иметь временную сложность O (n ^ 2), если центральный элемент выбран неудачно.

    **Параметры:**
    arr (List[Flight]): список объектов Flight для сортировки.

    **Возвращает:**
    List[Flight]: отсортированный список объектов Flight.
    """
    if len(flights) <= 1:
        return flights
    pivot = flights[len(flights) // 2]
    left = []
    right = []
    equal = []
    for flight in flights:
        comp = compare_flights(flight, pivot)
        if comp < 0:
            left.append(flight)
        elif comp > 0:
            right.append(flight)
        else:
            equal.append(flight)
    return quicksort_flights(left) + equal + quicksort_flights(right)
def bubble_sort_flights(flights):

    """
    **Bubble Sort Algorithm:**
    Алгоритм пузырьковой сортировки проходит по списку полетов, сравнивает соседние элементы и изменяет их местами, если они не удовлетвоярют условию сравнения.
    Этот процесс повторяется до тех пор, пока список не будет полностью отсортирован.

    
    Пузырьковая сортировка имеет сложность O(n^2), где n — количество элементов в списке.
    Обычно он считается из самых медленных алгоритмов сортировки и редко используется на практике для одного большого набора данных.

    **Параметры:**
    Flights (List[Flight]): список объектов Flight для сортировки.

    **Возвращает:**
    List[Flight]: отсортированный список объектов Flight.
    """
    n = len(flights)

    for i in range(n):
        for j in range(n-i-1):
            if compare_flights(flights[j], flights[j+1]) > 0:
                flights[j], flights[j+1] = flights[j+1], flights[j]

    return flights
def shaker_sort_flights(flights):
    """
    **Shaker Sort Algorithm:**
    Алгоритм шейкерной сортировки — это вариант алгоритма пузырьковой сортировки, в котором список сортируется в обоих направлениях, начиная с обоих концов списка.
    Он работает, перемещая самый большой элемент вправо и самый маленький элемент влево, пока весь список не будет отсортирован.

    Сортировка шейкером имеет временную сложность O(n^2), где n — количество элементов в списке.
    Это улучшенная версия пузырьковой сортировки, которая сортирует в обоих направлениях, но имеет ту же временную сложность для наихудшего случая.

    **Параметры:**
    Flights (List[Flight]): список объектов Flight для сортировки.

    **Возвращает:**
    List[Flight]: отсортированный список объектов Flight.
    """
    left = 0
    right = len(flights) - 1
    while left <= right:
        # Переместите наибольший элемент вправо
        for i in range(left, right):
            if compare_flights(flights[i], flights[i+1]) > 0:
                flights[i], flights[i+1] = flights[i+1], flights[i]
        right -= 1

        # Переместите наименьший элемент влево
        for i in range(right, left, -1):
            if compare_flights(flights[i-1], flights[i]) > 0:
                flights[i], flights[i-1] = flights[i-1], flights[i]
        left += 1

    return flights
