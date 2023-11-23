import os

def main():
    # Проверяем, существует ли канал
    if not os.path.exists("my_pipe"):
        os.mkfifo("my_pipe") # Создаём именованный канал, если он не существует

    while True:
        # Открываем канал для чтения
        read_pipe = os.open("my_pipe", os.O_RDONLY)

        # Читаем данные из канала
        message = os.read(read_pipe, 1024).decode('utf-8')
        print(f"Получено сообщение от клиента: {message}")

        # Закрываем канал после чтения сообщения
        os.close(read_pipe)

if __name__ == "__main__":
    main()