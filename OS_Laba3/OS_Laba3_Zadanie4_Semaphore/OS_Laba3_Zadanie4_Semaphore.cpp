#include <iostream>
#include <vector>
#include <Windows.h>
#include <thread>
#include <chrono>
#include <mutex>

const int MAX_VISITORS = 3;
HANDLE poolSemaphore;
std::mutex consoleMutex;

void Visitor(int visitorID) {
    {   
        setlocale(LC_ALL, "rus");
        std::lock_guard<std::mutex> lock(consoleMutex);
        std::cout << "Посетитель " << visitorID << " готовится плавать." << std::endl;
    }

    // Ожидание разрешения на посещение бассейна
    WaitForSingleObject(poolSemaphore, INFINITE); // поток ожидает, пока семафор не станет доступным (счетчик семафора не равен 0), 
    // что указывает на наличие свободных мест в бассейне.

    {
        std::lock_guard<std::mutex> lock(consoleMutex);
        std::cout << "Посетитель " << visitorID << " теперь плавает в бассейне!" << std::endl;
    }

    // Имитация пребывания в бассейне
    std::this_thread::sleep_for(std::chrono::seconds(2));

    {
        std::lock_guard<std::mutex> lock(consoleMutex);
        std::cout << "Посетитель " << visitorID << " покидает бассейн." << std::endl;
    }

    // Освобождение места в бассейне
    ReleaseSemaphore(poolSemaphore, 1, NULL);
}

int main() {
    // Создание семафора с начальным значением равным максимальному количеству посетителей
    poolSemaphore = CreateSemaphore(NULL, MAX_VISITORS, MAX_VISITORS, NULL); // Поток освобождает место в бассейне с помощью функции ReleaseSemaphore.

    std::vector<std::thread> visitors;
    const int NUM_VISITORS = 7;

    for (int i = 0; i < NUM_VISITORS; ++i) {
        visitors.emplace_back(Visitor, i + 1);
    }

    for (auto& visitorThread : visitors) {
        visitorThread.join();
    }

    // Закрытие дескриптора семафора
    CloseHandle(poolSemaphore);

    return 0;
}
