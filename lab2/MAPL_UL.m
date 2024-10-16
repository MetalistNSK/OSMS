function z = MAPL_UL()
    TxPowerUE = 24; %dBm
    FeederLoss = 2; %dB
    AntGainBS = 21; %dBi
    MIMOGain = 3; %dB
    IM = 1; %dB
    PenetrationM = 15; %dB
    NoiseFigure = 2.4; %dB 
    ThermalNoise = -174 + 10 * log10(10 * 10^6); %dB
    ReqiredSINR = 4; %dB
    RxSens = NoiseFigure + ThermalNoise + ReqiredSINR;
    z = TxPowerUE - FeederLoss + AntGainBS + MIMOGain - RxSens - IM - PenetrationM;
end
