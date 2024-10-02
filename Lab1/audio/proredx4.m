[y_orig, Fs] = audioread('labvoice.wav');
N = length(y_orig); % длина сигнала\


y_10 = downsample(y_orig, 4);
Fs_10 = Fs / 4;

zvuk = audioplayer(y_10,Fs_10);
play(zvuk);