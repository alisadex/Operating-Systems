import socket
import threading
from datetime import datetime


def start_logging_server():
    log_server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    log_server.bind(("127.0.0.1", 8888))
    log_server.listen(5)
    print("Logging server is listening on port 8888...")

    # Запись информации о запуске сервера
    with open('log.txt', 'a') as log_file:
        log_file.write(f"Server started at {datetime.now()}\n")

    while True:
        client, addr = log_server.accept()
        print(f"Accepted log connection from {addr[0]}:{addr[1]}")

        log_handler = threading.Thread(target=handle_log, args=(client, addr))
        log_handler.start()


def handle_log(log_client, addr):
    try:
        log_data = log_client.recv(1024).decode('utf-8')
        current_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        log_message = f"{current_time} - {log_data}"

        print("Received log data:")
        print(log_message)

        # Запись данных лога в файл
        with open('log.txt', 'a') as log_file:
            log_file.write(log_message + '\n')

        # Фиксация события подключения клиента
        with open('log.txt', 'a') as log_file:
            log_file.write(f"{current_time} - Accepted connection from {addr[0]}:{addr[1]}\n")
    except Exception as e:
        error_message = f"{current_time} - Error handling log: {str(e)}"
        print(error_message)

        # Фиксация события ошибки
        with open('log.txt', 'a') as log_file:
            log_file.write(error_message + '\n')
            log_file.write(f"{current_time} - disconnect error")
    finally:
        log_client.close()


if __name__ == "__main__":
    thread_logging_server = threading.Thread(target=start_logging_server)
    thread_logging_server.start()
    thread_logging_server.join()

