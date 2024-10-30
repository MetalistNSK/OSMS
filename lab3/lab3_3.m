% Определение массивов a и b
a = [0.3 0.2 -0.1 4.2 -2 1.5 0];
b = [0.3 4 -2.2 1.6 0.1 0.1 0.2];

% Определение длины массивов
N = length(a);

% Массив для хранения значений корреляции при каждом сдвиге
corr_values = zeros(1, N);

% Цикл для сдвига массива b и вычисления корреляции
for shift = 0:N-1
    b_shifted = circshift(b, shift); % Циклический сдвиг массива b
    corr_values(shift + 1) = sum(a .* b_shifted); % Вычисление корреляции
    disp(['Сдвиг: ', num2str(shift), ', Взаимная корреляция: ', num2str(corr_values(shift + 1))]);
end

% Построение графика зависимости корреляции от сдвига
figure;
plot(0:N-1, corr_values, '-o');
title('Зависимость корреляции от сдвига');
xlabel('Сдвиг');
ylabel('Корреляция');

% Определение максимальной корреляции и сдвига
[max_corr, max_shift] = max(corr_values); % Максимальная корреляция и сдвиг
disp(['Макс. корреляция: ', num2str(max_corr)]);
disp(['Сдвиг, при котором макс. корреляция: ', num2str(max_shift - 1)]);

% Построение графиков массива a и сдвинутого массива b
b_shifted = circshift(b, max_shift - 1); % Массив b, сдвинутый на оптимальный сдвиг

% Графики для массива a и сдвинутого b
figure;

% График для массива a
subplot(3,1,1);
plot(a, '-o');
title('Массив a');
xlabel('Индекс');
ylabel('Значение');

% График для сдвинутого массива b
subplot(3,1,2);
plot(b_shifted, '-o');
title(['Массив b, сдвинутый на ', num2str(max_shift-1)]);
xlabel('Индекс');
ylabel('Значение');

% Сравнение массивов a и сдвинутого b на одном графике
subplot(3,1,3);
plot(a, '-o', 'DisplayName', 'Массив a'); % Массив a
hold on;
plot(b_shifted, '-x', 'DisplayName', ['Сдвинутый массив b на ', num2str(max_shift-1)]); % Сдвинутый b
hold off;
title('Сравнение массивов a и сдвинутого b');
xlabel('Индекс');
ylabel('Значение');
legend;