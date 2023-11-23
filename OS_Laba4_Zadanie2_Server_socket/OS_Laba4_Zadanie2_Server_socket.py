import socket
import multiprocessing

# Функция обработки входящих соединений
def handle_client(client_socket):
    while True:
        try:
            data = client_socket.recv(1024)
            if not data:
                break
            
            # Получение сообщения
            message = data.decode('utf-8')
            print("Получено сообщение от клиента:", message)

            # Подтверждение получения клиента
            confirmation_message = "Сообщение получено: " + message
            client_socket.send(confirmation_message.encode('utf-8'))

        except ConnectionResetError:
            break

    client_socket.close()

if __name__ == "__main__":
    # Создаем сокет для сервера
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(("localhost", 12345)) # 127.0.0.1 = localhost
    server_socket.listen(2)  # Ожидание двух клиентов

    while True:
        print("Ожидание подключений...")
        client_socket, addr = server_socket.accept()
        print("Клиент подключен от", addr)

        # Создаем новый процесс для обработки клиента
        client_process = multiprocessing.Process(target=handle_client, args=(client_socket,))
        client_process.start()

