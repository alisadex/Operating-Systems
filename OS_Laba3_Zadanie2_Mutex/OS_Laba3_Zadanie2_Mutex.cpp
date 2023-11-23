#include <iostream>
#include <fstream>
#include <mutex>
#include <thread>

// Создаем мьютекс для синхронизации доступа к файлу
std::mutex file_mutex;

// Функция для записи в файл с использованием мьютекса
void write_to_file(const std::string& filename, const std::string& data) {
    std::lock_guard<std::mutex> lock(file_mutex);
    std::cout << std::this_thread::get_id() << " writes to a file '" << filename << "'" << std::endl;

    std::ofstream file(filename, std::ios::app);  // Открываем файл для записи
    if (file.is_open()) {
        file << data;
    }

    std::cout << std::this_thread::get_id() << " finished writing" << std::endl;
}

int main() {
    // Создаем файл "example.txt" (перезаписываем его, если он существует)
    std::string filename = "C:/Users/akvar/OneDrive/Рабочий стол/lab3OS/example.txt";
    std::ofstream init_file(filename);
    init_file.close();

    // Создаем два потока, каждый из которых вызовет функцию write_to_file
    std::thread thread1(write_to_file, filename, "Data from the Thread 1\nqwe\n");
    std::thread thread2(write_to_file, filename, "Data from the Thread 2\nrty\n");

    // Ждем, пока оба потока завершат свою работу
    thread1.join();
    thread2.join();

    std::cout << "All threads have shut down" << std::endl;

    return 0;
}
