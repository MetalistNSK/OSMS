import matplotlib.pyplot as plt

# Функция для чтения данных из файла
def read_data_from_file(filename):
    try:
        with open(filename, 'r') as file:
            data = [float(line.strip()) for line in file]  # Преобразование строк в числа
            return data
    except FileNotFoundError:
        print(f"Ошибка: Файл '{filename}' не найден.")
        return []
    except ValueError:
        print("Ошибка: Некорректные данные в файле.")
        return []

# Функция для построения графика
def plot_noise(data, title):
    if not data:
        print("Нет данных для построения графика.")
        return

    plt.figure(figsize=(12, 6))  # Устанавливаем размер графика
    plt.plot(data, linestyle='-', color='purple', label='Шумовые данные')
    plt.title(title, fontsize=16)
    plt.xlabel('Индекс', fontsize=12)
    plt.ylabel('Амплитуда', fontsize=12)
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend()
    plt.tight_layout()
    plt.show()

# Основной код
if __name__ == "__main__":
    filename = 'reference.txt'
    data = read_data_from_file(filename)
    plot_noise(data, 'График reference.txt')

    # plt.savefig("reference.png")