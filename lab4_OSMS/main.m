length = 31;
regX = [0, 1, 0, 1, 0];
regY = [1, 0, 0, 0, 1];
newRegX = [0, 1, 0, 1, 1]; 
newRegY = [0, 1, 1, 0, 0];

goldSequence = generateGoldSequence(regX, regY, length);
newGoldSequence = generateGoldSequence(newRegX, newRegY, length);

fprintf('Gold s original sequence: %s\n', num2str(goldSequence));

disp('Autocorrelation table:');
fprintf('Shift | Sequence                      | Autocorrelation\n');
fprintf('--------------------------------------------------------\n');
for shift = 0:length-1
    shiftedSeq = circshift(goldSequence, shift);

    autocorr = xcorr(goldSequence, shiftedSeq, 0, 'normalized'); 
    fprintf('%5d | %s | %9.7f\n', shift, mat2str(shiftedSeq), autocorr);
end

fprintf('New Gold Sequence: %s\n', num2str(newGoldSequence));

crossCorr = xcorr(goldSequence, newGoldSequence, 0, 'normalized');
disp('Cross-correlation:');
disp(crossCorr);

lags = -length+1:length-1;
autocorrelation = xcorr(goldSequence, 'normalized');
figure;
stem(lags, autocorrelation);
xlabel('Lag');
ylabel('Autocorr value');
title('Autocorr function');
grid on;


function goldSeq = generateGoldSequence(regX, regY, length)
    tapsX = [5, 2];
    tapsY = [5, 4, 3, 1];
    goldSeq = zeros(1, length);

    for i = 1:length
        bitX = getNextBit(regX, tapsX);
        bitY = getNextBit(regY, tapsY);
        
        goldSeq(i) = xor(bitX, bitY);

        regX = [bitX, regX(1:end-1)];
        regY = [bitY, regY(1:end-1)];
    end
end

function newBit = getNextBit(reg, taps)
    newBit = 0;
    for tap = taps
        newBit = xor(newBit, reg(tap));
    end
end

