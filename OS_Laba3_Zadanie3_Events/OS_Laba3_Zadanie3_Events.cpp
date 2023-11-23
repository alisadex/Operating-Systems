#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

// Создаем два события
std::condition_variable event_odd, event_even;
std::mutex mutex;
bool is_odd_turn = true;

void odd_thread() {
    for (int i = 1; i < 10; i += 2) {
        std::unique_lock<std::mutex> lock(mutex);
        event_odd.wait(lock, [] { return is_odd_turn; });
        std::cout << "Odd flow:" << i << std::endl;
        is_odd_turn = false;
        event_even.notify_one();
    }
}

void even_thread() {
    for (int i = 2; i <= 10; i += 2) {
        std::unique_lock<std::mutex> lock(mutex);
        event_even.wait(lock, [] { return !is_odd_turn; });
        std::cout << "Even flow:" << i << std::endl;
        is_odd_turn = true;
        event_odd.notify_one();
    }
}

int main() {
    // Создаем потоки
    std::thread t1(odd_thread);
    std::thread t2(even_thread);

    // Ждем завершения потоков
    t1.join();
    t2.join();

    return 0;
}
