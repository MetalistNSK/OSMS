function PL = WalfishIkegamiLOS(d)
    d = d / 10^3;
    f = 1.8 * 10 ^ 3; 
    PL = 42.6 + 20 * log10(f) + 26 * log10(d);
end