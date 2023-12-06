import os
import sys

def save_memory_map_to_file(process_id, output_file):
    try:
        # Путь к файлу maps процесса
        maps_file_path = f"/proc/{process_id}/maps"

        # Чтение информации о карте памяти
        with open(maps_file_path, 'r') as maps_file:
            maps_data = maps_file.read()

        # Запись информации в файл
        with open(output_file, 'w') as file:
            file.write(f"{process_id}: {process_name(process_id)}\n")
            file.write(maps_data)
        print(f"Карта памяти сохранена в файл {output_file}")
        
    except FileNotFoundError:
        print(f"Процесс с идентификатором {process_id} не найден.")
    except Exception as e:
        print(f"Произошла ошибка: {e}")

def process_name(process_id):
    # Получение имени процесса из /proc/[pid]/comm
    comm_file_path = f"/proc/{process_id}/comm"
    with open(comm_file_path, 'r') as comm_file:
        return comm_file.read().strip()

def main():
    if len(sys.argv) != 3:
        print("Использование: python3 process_memory_map.py <process_id> <output_file>")
        sys.exit(1)

    process_id = int(sys.argv[1])
    output_file = sys.argv[2]

    while os.path.exists(output_file):
        user_choice = input(f"Файл {output_file} уже существует. Вы хотите заменить его или сохранить в другой файл? (заменить/другой): ").lower()
        if user_choice == 'заменить':
            break
        elif user_choice == 'другой':
            new_name = input("Введите новое имя файла: ")
            output_file = new_name
        else:
            print("Некорректный выбор. Пожалуйста, выберите 'заменить' или 'другой'.")

    try:
        save_memory_map_to_file(process_id, output_file)
    except Exception as e:
        print(f"Произошла ошибка: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
