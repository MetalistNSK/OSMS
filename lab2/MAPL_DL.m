function z = MAPL_DL()
    TxPowerUE = 46; %dBm
    FeederLoss = 2; %dB
    AntGainBS = 21; %dBi
    MIMOGain = 3; %dB
    IM = 1; %dB
    PenetrationM = 15; %dB
    NoiseFigure = 6; %dB
    ThermalNoise = -174 + 10 * log10(20 * 10 ^ 6); %dB
    ReqiredSINR = 2; %dB
    RxSens = NoiseFigure + ThermalNoise + ReqiredSINR;
    z = TxPowerUE - FeederLoss + AntGainBS + MIMOGain - RxSens - IM - PenetrationM;
end