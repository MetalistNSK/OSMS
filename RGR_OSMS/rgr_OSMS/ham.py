import numpy as np
import matplotlib.pyplot as plt


file_paths = [
    "build/data/(1,3,1)extra_1.txt",
    "build/data/(1,3,2)extra_2.txt",
    # "build/data/(1,3,3)extra_3.txt"
]


def read_data(file_path):
   
    data = np.loadtxt(file_path, delimiter=";")
    x = data[:, 0]  
    y = data[:, 1]  
    return x, y


plt.figure(figsize=(10, 6))


for i, file_path in enumerate(file_paths):
    x, y = read_data(file_path)
    

    if i == 0:
        label = "Origin Message"
    elif i == 1:
        label = "Hamming"
    #elif i == 2:
     #   label = "Hamming without error improve"
    
    plt.plot(y, x, label=label)


plt.xlabel("Sigma")
plt.ylabel("Bit Error")
plt.title("Bit Error vs Sigma")
plt.legend()
plt.grid(True)


plt.show()
