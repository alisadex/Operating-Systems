#include <Windows.h>
#include <iostream>
#include <thread>

// Глобальные переменные
int accountBalance = 0;
HANDLE mutex;


/* 
Функция блокирует мьютекс с помощью WaitForSingleObject перед выполнением операции.
WaitForSingleObject(mutex, INFINITE); блокирует поток, который ее вызывает, до тех пор, пока мьютекс (mutex) не будет доступен. 
Он может ждать бесконечно долго, пока не получит сигнал о том, что мьютекс доступен и его можно использовать.
*/

void PerformTransaction(int amount, const char* action) {
    WaitForSingleObject(mutex, INFINITE);

    // Имитация времени выполнения операции
    Sleep(100);

    accountBalance += amount;
    setlocale(LC_ALL, "rus");
    std::cout << "Выполненная операция: " << action << ". Новый баланс: " << accountBalance << std::endl;

    ReleaseMutex(mutex);
}


// Функции потоков для депозита и снятия средств:
void DepositThread() {
    for (int i = 0; i < 5; ++i) {
        int depositAmount = rand() % 100 + 1;
        PerformTransaction(depositAmount, "депозит");
    }
}

void WithdrawThread() {
    for (int i = 0; i < 5; ++i) {
        int withdrawAmount = rand() % 50 + 1;
        PerformTransaction(-withdrawAmount, "снятие");
    }
}

int main() {
    // Создание мьютекса
    /* 1 аргумент - атрибуты безопасности мьютекса
    *  2 аргумент - должен ли мьютекс быть в исходном заблокированном состоянии
    *  3 аргумент - имя мьютекса (мьютекс не имеет имени в нашем случае(он анонимный))
    */
    mutex = CreateMutex(NULL, FALSE, NULL); 

    std::thread depositThread(DepositThread);
    std::thread withdrawThread(WithdrawThread);

    depositThread.join();
    withdrawThread.join();

    // Закрытие мьютекса
    CloseHandle(mutex);

    return 0;
}
