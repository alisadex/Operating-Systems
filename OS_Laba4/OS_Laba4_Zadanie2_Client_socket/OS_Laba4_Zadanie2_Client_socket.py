import socket

if __name__ == "__main__":
    # Создаем сокет для клиента
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect(("localhost", 12345)) # 127.0.0.1 = localhost

    while True:
        message = input("Введите сообщение (или 'exit' для завершения): ")
        if message == 'exit':
            break
        client_socket.send(message.encode('utf-8'))
        
        # Подтверждение получения сообщения сервером
        confirmation_message = client_socket.recv(1024)
        response = confirmation_message.decode('utf-8')
        print("От сервера получено уведомление:", response)

