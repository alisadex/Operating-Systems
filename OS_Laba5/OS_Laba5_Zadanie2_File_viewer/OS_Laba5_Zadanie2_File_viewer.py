def read_file(file_name):
    try:
        # Попытка открыть файл для чтения
        file = open(file_name, 'r')
        # Чтение содержимого файла
        content = file.read()
        # Закрытие файла
        file.close()
        return content
    except IOError:
        # Обработка ошибки при открытии файла
        print("Ошибка открытия файла.")
        return None

def display_content(content, group_size):
    if content is not None:
        if group_size == 0:
            # Если размер группы равен 0, выводим весь текст
            print(content)
        else:
            # Разбиваем содержимое файла на строки
            lines = content.split('\n')
            # Итерируем по строкам с шагом group_size
            for i in range(0, len(lines), group_size):
                # Выбираем группу строк
                group = lines[i:i + group_size]
                # Выводим группу строк на экран
                print('\n'.join(group))
                # Если не достигнут конец файла, ожидаем нажатия Enter
                if i + group_size < len(lines):
                    input("Нажмите Enter для продолжения...")

if __name__ == "__main__":
    import sys

    if len(sys.argv) != 3:
        # Проверка наличия двух аргументов командной строки
        print("Использование: python program_name.py <file_name> <group_size>")
        sys.exit(1)

    file_name = sys.argv[1]
    try:
        group_size = int(sys.argv[2])
    except ValueError:
        # Обработка ошибки при некорректном вводе размера группы
        print("Второй аргумент должен быть целым числом.")
        sys.exit(1)

    content = read_file(file_name)
    if content is not None:
        # Вывод содержимого файла на экран с учетом размера группы строк
        display_content(content, group_size)
