% Параметры времени и частоты
f1 = 10; 
f2 = f1 + 4;
f3 = f1 * 2 + 1;
t = (0:100-1)/100; % временной интервал

% Опред. сигналов
s1 = cos(2 * pi * f1 * t);
s2 = cos(2 * pi * f2 * t);
s3 = cos(2 * pi * f3 * t);

% Сигналы a и b
sig_a = 5 * s1 + 4 * s2 + s3;
sig_b = 3*s1 + s3;

% Выч. корреляции
correlation_ab = sum(sig_a .* sig_b);
norm_correlation_ab = sum(sig_a .* sig_b) / (sqrt(sum(sig_a.^2)) * sqrt(sum(sig_b.^2)));

disp('Корр. между сигналами a и b:');
disp(correlation_ab);
disp('Нормал. корр. между сигналами a и b:');
disp(norm_correlation_ab);