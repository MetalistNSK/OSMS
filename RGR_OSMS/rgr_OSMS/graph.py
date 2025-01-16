import matplotlib.pyplot as plt
import numpy as np

# Функция для чтения данных из файла
def read_data_from_file(filename):
    try:
        with open(filename, 'r') as file:
            data = file.read().strip()  # Убираем лишние пробелы и символы новой строки
            return [int(bit) for bit in data if bit in '01']  # Преобразуем только 0 и 1 в список
    except FileNotFoundError:
        print(f"Ошибка: Файл '{filename}' не найден.")
        return []
    except ValueError:
        print("Ошибка: Некорректные данные в файле.")
        return []

# Функция для построения прямоугольного импульса
def plot_rectangular_pulse(data, title, color, subplot_position):
    if not data:
        print(f"Нет данных для построения графика: {title}.")
        return

    # Подготовка данных для построения прямоугольного импульса
    x = np.repeat(range(len(data)), 2)  # Дублирование индексов
    y = np.repeat(data, 2)  # Дублирование значений (0 или 1)

    # Смещение x для создания ступенек
    x = np.insert(x, 0, x[0])  # Добавляем начальный элемент
    y = np.insert(y, 0, y[0])  # Добавляем начальный уровень

    # Построение графика
    plt.subplot(2, 1, subplot_position)
    plt.step(x, y, where='post', color=color, label=title)
    plt.title(title, fontsize=16)
    plt.xlabel('Индекс', fontsize=12)
    plt.ylabel('Уровень (0 или 1)', fontsize=12)
    plt.ylim(-0.5, 1.5)
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend()

# Основной код
if __name__ == "__main__":
    # Чтение данных из файлов
    buffer_data = read_data_from_file('buffer.txt')
    buffer_samples_data = read_data_from_file('buffer_samples.txt')

    # Построение графиков
    plt.figure(figsize=(12, 10))
    plot_rectangular_pulse(buffer_data, 'Прямоугольный импульс: buffer.txt', 'blue', 1)
    plot_rectangular_pulse(buffer_samples_data, 'Прямоугольный импульс: buffer_samples.txt', 'green', 2)
    #plt.savefig("graph_output.png")
    plt.tight_layout()
    plt.show()



#plt.savefig("graph_output.png")  # Сохраняем график в файл
# plt.show()  # Опционально: вызов можно оставить для отладки
