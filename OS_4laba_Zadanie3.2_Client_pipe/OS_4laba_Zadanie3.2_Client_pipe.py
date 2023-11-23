import os

def main():
    # Проверяем, существует ли канал
    if not os.path.exists("my_pipe"):
        os.mkfifo("my_pipe") # Создаём именованный канал, если он не существует

    # Открываем канал для записи
    write_pipe = os.open("my_pipe", os.O_WRONLY)

    # Считываем данные из stdin (ввод с клавиатуры) и отправляем в канал
    message = input("Введите текст для отправки в канал: ")
    os.write(write_pipe, message.encode('utf-8'))

    # Закрываем канал
    os.close(write_pipe)

if __name__ == "__main__":
    main()
