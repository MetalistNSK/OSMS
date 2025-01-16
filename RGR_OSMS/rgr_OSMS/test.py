import numpy as np
import matplotlib.pyplot as plt

# Генерация псевдослучайной GOLD-последовательности
def generate_gold_sequence(length=31):
    return np.random.randint(0, 2, length).tolist()

# Кодирование сообщения в ASCII
def encode_to_ascii(message):
    bit_sequence = []
    for char in message:
        bits = bin(ord(char))[2:].zfill(8)
        bit_sequence.extend([int(b) for b in bits])
    return bit_sequence

# Кодирование Хэмминга (7, 4)
def encode_hamming(bit_sequence):
    encoded = []
    for i in range(0, len(bit_sequence), 4):
        block = bit_sequence[i:i+4]
        # Вставляем контрольные биты (позиции: 1, 2, 4)
        p1 = block[0] ^ block[1] ^ block[3]  # Первый контрольный бит
        p2 = block[0] ^ block[2] ^ block[3]  # Второй контрольный бит
        p3 = block[1] ^ block[2] ^ block[3]  # Третий контрольный бит
        encoded.extend([p1, p2, block[0], p3, block[1], block[2], block[3]])
    return encoded

# Генерация CRC (простейший пример)
def generate_crc(bit_sequence, length=16):
    crc = sum(bit_sequence) % (1 << length)  # Простейшая контрольная сумма
    return [int(b) for b in bin(crc)[2:].zfill(length)]

# Комбинирование битовых последовательностей
def combine_sequences(*sequences):
    combined = []
    for seq in sequences:
        combined.extend(seq)
    return combined

# Генерация белого шума
def generate_white_noise(length, mu, sigma):
    return np.random.normal(mu, sigma, length)

# Интерпретация сигнала
def interpret_signal(signal, threshold=0.18):
    return [1 if s > threshold else 0 for s in signal]

# Декодирование Хэмминга (7, 4)
def decode_hamming(encoded_sequence):
    decoded = []
    # Проверяем длину полученной последовательности
    for i in range(0, len(encoded_sequence), 7):
        block = encoded_sequence[i:i+7]
        
        # Проверка на корректную длину блока
        if len(block) < 7:
            continue  # Пропускаем блоки, которые не имеют 7 бит

        # Проверка контрольных бит
        p1 = block[0] ^ block[2] ^ block[4] ^ block[6]
        p2 = block[1] ^ block[2] ^ block[5] ^ block[6]
        p3 = block[3] ^ block[4] ^ block[5] ^ block[6]
        error_position = p1 * 1 + p2 * 2 + p3 * 4
        
        # Если есть ошибка, исправляем
        if error_position != 0:
            block[error_position - 1] ^= 1
        
        # Декодируем данные (извлекаем 4 бита данных)
        decoded.extend([block[2], block[4], block[5], block[6]])
    return decoded

# Подсчет битовых ошибок
def count_bit_errors(original, received):
    errors = sum(1 for o, r in zip(original, received) if o != r)
    return errors

# Основная функция
if __name__ == "__main__":
    message = "Nikita Makedon"
    bit_sequence = encode_to_ascii(message)
    bit_sequence_hamming = encode_hamming(bit_sequence)

    crc = generate_crc(bit_sequence)
    crc_hamming = generate_crc(bit_sequence_hamming)

    gold_sequence = generate_gold_sequence()

    combined_seq = combine_sequences(gold_sequence, bit_sequence, crc)
    combined_seq_hamming = combine_sequences(gold_sequence, bit_sequence_hamming, crc_hamming)

    mu = 0.0
    sigmas = np.arange(0.0, 0.3, 0.0005)
    num_runs = 10  # Количество прогонов для усреднения
    errors_avg = []
    errors_hamming_avg = []

    for sigma in sigmas:
        total_errors = 0
        total_errors_hamming = 0
        
        for _ in range(num_runs):
            # Генерация шума с одинаковой длиной для обоих сигналов
            noise = generate_white_noise(len(combined_seq), mu, sigma)
            noise_hamming = generate_white_noise(len(combined_seq_hamming), mu, sigma)

            # Добавляем шум
            signal = np.array(combined_seq) + noise
            signal_hamming = np.array(combined_seq_hamming) + noise_hamming

            received_sequence = interpret_signal(signal)
            received_sequence_hamming = interpret_signal(signal_hamming)

            # Декодирование и исправление ошибок для Хэмминга
            decoded_hamming = decode_hamming(received_sequence_hamming)

            bit_errors = count_bit_errors(combined_seq, received_sequence)
            bit_errors_hamming = count_bit_errors(bit_sequence, decoded_hamming)

            total_errors += bit_errors
            total_errors_hamming += bit_errors_hamming

        # Усреднение ошибок по количеству прогонов
        errors_avg.append(total_errors / num_runs)
        errors_hamming_avg.append(total_errors_hamming / num_runs)

    # Построение графиков
    plt.plot(sigmas, errors_avg, label="Without Hamming")
    plt.plot(sigmas, errors_hamming_avg, label="With Hamming")
    plt.xlabel("Sigma")
    plt.ylabel("Average Bit Errors")
    plt.title("Average Bit Errors vs. Sigma (Averaged over multiple runs)")
    plt.legend()
    plt.grid(True)
    plt.show()
