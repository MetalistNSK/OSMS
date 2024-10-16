function PL = FSPM(d)
    f_Hz = 1.8e9;
    PL = 20 * log10( 4 * pi * d * f_Hz / 3e8 );
end