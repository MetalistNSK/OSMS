function [PL_DU, PL_U] = OkomuraHata(d) 
    f_MHz = 1.8e9/1e6; % ГГц в МГц
    d_Km = d / 1e3; % M to Km

    % т.к. f = 1.8 ГГц = 1800Мгц
    A = 46.3; B = 33.9; 

    % random [30; 200] m 
    hBS = 30; 
    % random [1; 10] m 
    hms = 1.5; 

    % плотная застройка
    Lclutter_du = 3; 

    % город
    Lclutter_u = 0; 

    % DU и U 
    a = 3.2 * ceil(log10(11.75 * hms)) ^ 2 - 4.97; 

    if d_Km >= 1 
        s = 44.9 - 6.55 * log10(f_MHz);
    else 
        s = (47.88 + 13.9 * log10(f_MHz) - 13.9 * log10(hBS)) * 1 / (log10(50));
    end 

    PL_DU = A + B * log10(f_MHz) - 13.82*log10(hBS) - a + s * log10(d_Km) + Lclutter_du; 
    PL_U = A + B * log10(f_MHz) - 13.82*log10(hBS) - a + s * log10(d_Km) + Lclutter_u;
end
