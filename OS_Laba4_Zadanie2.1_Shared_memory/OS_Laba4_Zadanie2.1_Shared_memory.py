from multiprocessing import shared_memory, Process
import os
import time

message_size = 256  # Размер сообщения в байтах

# Функция для отправки сообщений
def send_message(shared_data, recipients, message):
    message_bytes = f"{message} [{os.getpid()}]".encode() # Формируем сообщение
    for recipient in recipients: # Для каждого адресата
        offset = recipient * message_size # Вычисляем смещение в разделяемой памяти на основе размера сообщения и номера адресата
        shared_data.buf[offset:offset+len(message_bytes)] = message_bytes # Записываем сообщение в разделяемую память
        # Добавляем нулевые байты, чтобы очистить оставшееся место в разделяемой памяти
        shared_data.buf[offset+len(message_bytes):offset+message_size] = b'\0' * (message_size - len(message_bytes))

# Функция для приёма сообщений
def receive_message(shared_data, confirmation_data, recipient):
    offset = recipient * message_size # Вычисляем смещение для данного адресата в разделяемой памяти
    message_bytes = shared_data.buf[offset:offset+message_size].tobytes() # Извлекаем байты сообщения из разделяемой памяти на основе смещения и размера сообщения
    message = message_bytes.split(b'\0', 1)[0].decode() # Декодируем байты сообщения, разделяемые символов '\0', чтобы получить текст сообщения
    if message: # Если сообщение непустое
        print(f"Процесс [{os.getpid()}] получил сообщение: {message}") # Выводим сообщение с PID
        shared_data.buf[offset:offset+message_size] = b'\0' * message_size # Очищаем разделяемую память
        confirmation_data.buf[recipient] = 1 # Устанавливаем подтверждение в разделяемой памяти для данного адресата

if __name__ == '__main__':
    num_processes = 2 # Количество процессов
    recipients = [0, 1] # Список адресатов
    
    # Создаем разделяемую память для сообщений
    shared_data = shared_memory.SharedMemory(create=True, size=message_size * num_processes)
    
    # Создаем разделяемую память для подтверждений
    confirmation_data = shared_memory.SharedMemory(create=True, size=num_processes)
    
    # Инициализируем процессы-ведомые
    processes = []
    for recipient in recipients: # Для каждого адресата
        # Создаём процесс с целью receive_message (каждый процесс получает доступ к разделяемой памяти)
        process = Process(target=receive_message, args=(shared_data, confirmation_data, recipient))
        process.start()
        processes.append(process)
    
    # Отправляем сообщение сразу нескольким адресатам
    send_message(shared_data, recipients, f"Привет, это сообщение от ведущего процесса")
    
    # Ожидаем подтверждения приема сообщений
    while not all(confirmation_data.buf):
        time.sleep(1)
    
    print("Все сообщения успешно доставлены и подтверждены.")
    
    # Останавливаем процессы-ведомые
    for process in processes:
        process.terminate()
    
    # Закрываем разделяемую память
    shared_data.close() # Закрытие объекта разделяемой памяти
    shared_data.unlink() # Удаление объекта разделяемой памяти
    confirmation_data.close() # Закрытие объекта разделяемой памяти
    confirmation_data.unlink() # Удаление объекта разделяемой памяти