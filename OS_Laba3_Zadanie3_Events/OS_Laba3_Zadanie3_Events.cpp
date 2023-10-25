#include <Windows.h>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

const int NUM_CARS = 5;
HANDLE startEvent;
std::mutex consoleMutex; // мьютекс для синхронизации доступа к консоли из нескольких потоков

void Racer(int racerID) {
    {   setlocale(LC_ALL, "rus");
        /*Создание объекта lock_guard, который автоматически захватывает мьютекс consoleMutex.
        Это сделано для синхронизации доступа к консоли из разных потоков. */
        std::lock_guard<std::mutex> lock(consoleMutex);
        std::cout << "Гонщик #" << racerID << " готов стартовать." << std::endl;
    }

    WaitForSingleObject(startEvent, INFINITE); // Ожидание события начала гонки

    {   
        std::lock_guard<std::mutex> lock(consoleMutex);  
    
        std::cout << "Гонщик #" << racerID << " начал стартовать!" << std::endl;
    }
}

int main() {
    // Создание автоматического события начала гонки
    startEvent = CreateEvent(NULL, TRUE, FALSE, NULL); // Событие устанавливается в несигнализированное (FALSE) состояние.

    // Создание потоков для гонщиков
    std::vector<std::thread> racers; // Создание вектора для хранения объектов потоков гонщиков.
    for (int i = 0; i < NUM_CARS; ++i) {
        racers.emplace_back(Racer, i + 1); // Создает и добавляет новый элемент в конец вектора
    }

    {   
        setlocale(LC_ALL, "rus");
        std::lock_guard<std::mutex> lock(consoleMutex);
        std::cout << "Внимание! Гонка начинается!" << std::endl;
    }

    // Сигнализация события начала гонки
    SetEvent(startEvent);

    // Ожидание завершения всех потоков гонщиков
    for (auto& racerThread : racers) {
        racerThread.join();
    }

    // Закрытие события
    CloseHandle(startEvent);

    return 0;
}
