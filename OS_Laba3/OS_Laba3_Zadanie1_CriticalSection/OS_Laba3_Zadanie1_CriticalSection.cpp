#include <Windows.h>
#include <iostream>
#include <vector>
#include <thread>

// Глобальные переменные
int goodsCount = 0;
int goodsBought = 0;
int goodsSupplied = 0;
CRITICAL_SECTION goodsCountCriticalSection; // Cоздание критической секции

// Функция для изменения счетчика товаров внутри критической секции
void ChangeGoodsCount(int changeAmount, const char* action) {
    EnterCriticalSection(&goodsCountCriticalSection);
    goodsCount += changeAmount;
    setlocale(LC_ALL, "rus");
    if (changeAmount > 0) {
        goodsSupplied += changeAmount;
    }
    else {
        goodsBought -= changeAmount;
    }
    std::cout << "Количество товаров изменено. Новое количество: " << goodsCount << " (Продано: " << goodsBought << ", поставлено: " << goodsSupplied << ") Действие: " << action << std::endl;
    LeaveCriticalSection(&goodsCountCriticalSection);
    // Только один поток может изменять goodsCount в определенный момент времени
}


/*
Функции, моделирующие поток продаж и поставок.
Они вызывают функцию ChangeGoodsCount для изменения счетчика товаров в критической секции.
*/

void SalesThread() {
    for (int i = 0; i < 5; ++i) {
        // Моделируем продажу товаров
        Sleep(100); // Имитация времени продажи
        ChangeGoodsCount(-1, "Продано (-1)");
    }
}

void SupplyThread() {
    for (int i = 0; i < 10; ++i) {
        // Моделируем поставку товаров
        Sleep(200); // Имитация времени поставок
        ChangeGoodsCount(2, "Поставлено (+2)");
    }
}

int main() {
    // Инициализация критической секции
    InitializeCriticalSection(&goodsCountCriticalSection);

    // Создаем потоки для продаж и поставок
    std::thread salesThread(SalesThread);
    std::thread supplyThread(SupplyThread);

    /*
    Ожидание завершения потоков
    Join — это метод синхронизации, который блокирует вызывающий поток (т. е. поток, вызывающий метод), пока не завершится поток, метод которого Join вызывается.
    */

    salesThread.join();
    supplyThread.join();

    // Уничтожение критической секции после завершения работы потоков.
    DeleteCriticalSection(&goodsCountCriticalSection);

    return 0;
}
