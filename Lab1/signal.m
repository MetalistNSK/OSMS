clear; 

f = 5; 
y = @(t) cos(6 * pi * f * t) + sin(4 * pi * f * t) + sin(10 * t); 

fs = 1000;
t = 0:1/fs:1; 

figure; 
    plot(t, y(t));

    xlabel('time (s)');
    ylabel('Amplitude');
    grid on;
     title("График сигнала");

% Задаём частоту дискретизации и разбиваем ось времени
fs = 15 * 2; 
t = 0:1/fs:1;

% Сохраняем значение функций в массив
samples = arrayfun(y, t);

draw_samples(samples, t); 

ppf = fft(samples); 
opf = ifft(ppf); 

draw_ppf_spectre(t, ppf);

fprintf("Ширина спектра: %.2f\n", max(abs(ppf(:))));
fprintf('Занято памяти (Кб): %.2f\n', whos("ppf").bytes / 1024);


% задание 7
fs_4x = fs * 4; 
t_4x = 0:1/fs_4x:1;

samples_4x = arrayfun(y, t_4x); 

ppf_4x = fft(samples_4x); 
opf_4x = ifft(ppf_4x); 

draw_diff_4x(t, t_4x, samples, samples_4x, ppf, ppf_4x, opf, opf_4x);
draw_diff_opf_signal_4x(samples_4x, opf_4x, t_4x); 

fprintf("Ширина спектра: %.2f\n", max(abs(ppf_4x(:))));
fprintf('Занято памяти (Кб): %.2f\n', whos("ppf_4x").bytes / 1024);

quantization_test(y);

function quantization_test(y)
    fs = 5000;
    t = 0:1/fs:1; 

    original_signal = y(t); 

    bit_depths = [3, 4, 5, 6];

    for bits = bit_depths
        quantized_signal = quantize(original_signal, bits);
        
        Y = fft(original_signal);
        Y_quantized = fft(quantized_signal);
        
        amplitude_spectrum = abs(Y);
        amplitude_spectrum_quantized = abs(Y_quantized);
        
        quantization_error = original_signal - quantized_signal;
        mean_quantization_error = mean(abs(quantization_error));
        
        f = (0:length(original_signal)-1) * (fs / length(original_signal));
        
        figure;
        plot(f, amplitude_spectrum, 'b', 'DisplayName', 'Оригинальный сигнал');
        hold on;
        plot(f, amplitude_spectrum_quantized, 'r', 'DisplayName', sprintf('Квантованный сигнал (биты = %d)', bits));
        hold off;
        title(sprintf('Сравнение амплитудных спектров для %d бит', bits));
        xlabel('Частота (Гц)');
        ylabel('Амплитуда');
        xlim([0 fs/2]);
        legend show;
        grid on;
        
        fprintf('Средняя ошибка квантования для %d бит: %.4f\n', bits, mean_quantization_error);
    end
end

function draw_diff_4x(t, t_4x, samples, samples_4x, ppf, ppf_4x, opf, opf_4x)
    figure; 
        plot(t, samples, 'DisplayName', 'Сигнал с частотой fs');
        hold on; 
            plot(t_4x, samples_4x, 'DisplayName', 'Сигнал с частотой 4fs');
        hold off; 
        xlabel('time (s)');
        ylabel('Amplitude');
        grid on; 
        legend('show');
        title("Дискретизированный сигнал");
end 

function draw_diff_opf_signal_4x(samples, opf, t) 
    figure; 
        subplot(2,1,1); 
        plot(t, samples); 
        xlabel('time (s)');
        ylabel('Amplitude');
        title("Разница между обратно преобразованным цифровым сигналом и первонач. сигналом")

        subplot(2,1,2); 
        plot(t, opf); 
        xlabel('time (s)');
        ylabel('Amplitude');
end 

function draw_fourier(reference_opf, opf, t) 
    figure; 
        subplot(2,1,1); 
        plot(t, reference_opf); 
        xlabel('time (s)');
        ylabel('Amplitude');
        title("График восстановления ifft/fft")
end 

function draw_diff_opf_signal(samples, opf, t) 
    figure; 
        subplot(2,1,1); 
        plot(t, samples); 
        xlabel('time (s)');
        ylabel('Amplitude');
        title("Разница между обратно преобразованным цифровым сигналом и сигналом")

        subplot(2,1,2); 
        plot(t, opf); 
        xlabel('time (s)');
        ylabel('Amplitude');
        title("Разница между обратно преобразованным цифровым сигналом и сигналом")
end 

function draw_ppf_spectre(t, ppf)
    figure;
        plot(abs(ppf));
        xlabel('Frequency');
        ylabel('Amplitude');
        grid on;
        title("Спектр");
end 


function draw_samples(samples, t)
    figure; 
        plot(t, samples);
        xlabel('time (s)');
        ylabel('Amplitude');
        grid on;
        title("Дискретизированный сигнал");
end 

function quantized_signal = quantize(signal, bits)
    max_value = max(signal); % Максимальная амплитуда синусоиды
    min_value = min(signal); % Минимальная амплитуда синусоиды
    
    % Определите количество уровней квантования
    levels = 2^bits;
    
    % Масштабируйте сигнал для диапазона уровней
    scaled_signal = (signal - min_value) / (max_value - min_value) * (levels - 1);
    
    % Квантование сигналов
    quantized_scaled_signal = round(scaled_signal); % Округление до ближайшего уровня
    quantized_scaled_signal(quantized_scaled_signal >= levels) = levels - 1; % Ограничьте уровень
    quantized_signal = quantized_scaled_signal / (levels - 1) * (max_value - min_value) + min_value; % Обратное масштабирование
end