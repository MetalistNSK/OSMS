function PL = UMiNLOS(d)
    f_GHz = 1.8; 
    PL = 26 * log10(f_GHz) + 22.7 + 36.7 * log10(d);
end