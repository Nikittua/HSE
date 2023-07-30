
from datetime import datetime
from typing import List
import time
from Flight_generator import *
from algorithms import *


class Flight:
    """
    Описывает рейс с номером рейса, авиакомпанией, датой прибытия, временем прибытия и количеством пассажиров. 
    В данном классе реализоываны перегрузки операторов > < >= <= ==

     **Атрибуты:**
         Flight_number (str): номер рейса.
         airline  (str): Название авиакомпании.
         arrival_date  (str): Дата прибытия в формате «ГГГГ-ММ-ДД».
         arrival_time  (str): время прибытия в формате «ЧЧ:ММ».
         passengers  (int): количество пассажиров на рейсе.

    """
    def __init__(self, flight_number, airline, arrival_date, arrival_time, passengers):
        self.flight_number = flight_number
        self.airline = airline
        self.arrival_date = arrival_date
        self.arrival_time = arrival_time
        self.passengers = passengers

    def __lt__(self, other):
        if self.arrival_date != other.arrival_date:
            return self.arrival_date < other.arrival_date
        if self.arrival_time != other.arrival_time:
            return self.arrival_time < other.arrival_time
        if self.airline != other.airline:
            return self.airline < other.airline
        return self.passengers > other.passengers

    def __eq__(self, other):
        return (self.arrival_date == other.arrival_date and
                self.arrival_time == other.arrival_time and
                self.airline == other.airline and
                self.passengers == other.passengers)
    def __repr__(self):
        return f"{self.flight_number}, {self.airline}, {self.arrival_date}, {self.arrival_time}, {self.passengers}"

    def __gt__(self, other):
        return not self.__lt__(other) and not self.__eq__(other)

    def __le__(self, other):
        return self.__eq__(other) or self.__lt__(other)

    def __ge__(self, other):
        return self.__eq__(other) or self.__gt__(other)
