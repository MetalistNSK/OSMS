import matplotlib.pyplot as plt
import numpy as np
file_path = "buffer_10.txt"



# Чтение данных

with open(file_path, "r") as file:

    data = file.read().strip()



# Преобразование данных в числовой массив

binary_data = np.array([int(bit) for bit in data])



# Построение графика

plt.figure(figsize=(15, 5))

plt.plot(binary_data, drawstyle='steps-post')

plt.title("График upsampling 10")

plt.xlabel("Индекс")

plt.ylabel("Значение (0 или 1)")

plt.grid()

plt.show()