import socket
import psutil
lock_socket = None
import threading
def get_thread_info():
    # Получение информации о модулях сервера
    pid = psutil.Process()
    modules = pid.memory_maps()
    count = 0
    for modules in modules:
        count += 1
    return f"Количество модулей серверного процесса: {count}"

def handle_client(client_socket,client_address):
    try:
        # Получение информации о системе
        system_info = f"Количество потоков серверного процесса : {psutil.cpu_count(logical=False)}\n"

        # Получение информации о потоках сервера
        server_threads_info = get_thread_info()
        server_info = "".join(server_threads_info)
        log_message = f"Connection from {client_address[0]}:{client_address[1]} on Server 2\n{system_info}{server_info}"
        client_socket.send((system_info + server_info).encode())
        # Отправка лога на сервер логирования
        send_log_to_logging_server(log_message)

    except Exception as e:
        print(f"Error handling client: {str(e)}")
    finally:
        client_socket.close()
def start_server():
    global lock_socket
    try:
        lock_socket = socket.create_connection(("localhost", 8084))
    except OSError:
        pass
    else:
        print("Server is already running.")
        exit(1)
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        server.bind(("127.0.0.1", 8082))
    except:
        log_message1 = f"Server 2 SHUT DOWN!"
        send_log_to_logging_server(log_message1)
        exit(1)
    else:
        server.listen(5)
        print("Server 2 is listening on port 8082...")

    try:
        while True:
            client, addr = server.accept()
            print(f"Accepted connection from {addr[0]}:{addr[1]} on Server 2")
            client_handler = threading.Thread(target=handle_client, args=(client,addr))
            client_handler.start()
    except KeyboardInterrupt:
        print("Server 2 is shutting down...")
    finally:
        server.close()

def send_log_to_logging_server(log_message):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as log_client_socket:
        log_client_socket.connect(('127.0.0.1', 8888))
        log_client_socket.send(log_message.encode('utf-8'))
if __name__ == "__main__":
    thread_server2 = threading.Thread(target=start_server)
    thread_server2.start()
    thread_server2.join()

