% Определение массивов a и b
a = [0.3 0.2 -0.1 4.2 -2 1.5 0];
b = [0.3 4 -2.2 1.6 0.1 0.1 0.2];

% Построение графиков a и b
figure;

% График для массива a
subplot(2,1,1);
plot(a, '-o');
title('Массив a');
xlabel('Индекс');
ylabel('Значение');

% График для массива b
subplot(2,1,2);
plot(b, '-o');
title('Массив b');
xlabel('Индекс');
ylabel('Значение');

% Вычисление взаимной корреляции
corr_ab = sum(a .* b); % Взаимная корреляция
disp(['Взаимная корреляция: ', num2str(corr_ab)]);