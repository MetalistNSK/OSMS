import matplotlib.pyplot as plt
import numpy as np
file_path = "ch_autocorr_0.3.txt"

# Чтение данных
try:
    with open(file_path, "r") as file:
        data = file.readlines()
    # Преобразование данных в числовой массив
    numerical_data = np.array([float(value.strip()) for value in data])

    # Вычисление автокорреляции
    autocorr = np.correlate(numerical_data, numerical_data, mode='full')
    lags = np.arange(-len(numerical_data) + 1, len(numerical_data))

    # Построение графика
    plt.figure(figsize=(12, 6))
    plt.plot(lags, autocorr, label="Автокорреляция")
    plt.title("График автокорреляции по данным из файла ch_autocorr_0.3")
    plt.xlabel("Лаг")
    plt.ylabel("Значение автокорреляции")
    plt.axhline(0, color='black', linewidth=0.8, linestyle='--')
    plt.grid()
    plt.legend()
    plt.show()

except FileNotFoundError:
    print("Файл ch_autocorr_0.3 не найден. Загрузите файл, чтобы построить график.")
