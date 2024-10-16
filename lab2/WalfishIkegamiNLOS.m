function Lnlos = WalfishIkegamiNLOS(d)
    d = d / 10^3;
    f = 1.8 * 10^3; 
    % random [0; 90] fi 
    fi = 40; % Градусах

    % random [10; 30] m 
    w = 20;

    % random [40; 150] m
    b = 110; 

    % random [30; 200] m 
    hBS = 150; 
    % random [1; 10] m 
    hms = 5; 

    dh = hBS - hms; 

    L0 = 32.44 + 20 * log10(f) + 20 * log10(d); 
   
    if (hBS > dh)
        L11 = -18 * log10(1 + hBS - dh);
    else 
        L11 = 0;
    end 

    if (hBS > dh)
        ka = 54; 
    else 
        if (hBS <= dh) 
            if (d > 0.5)
                ka = 54 - 0.8 * (hBS - dh);
            else 
                ka = 54 - 0.8 * (hBS - dh) * (d / 0.5);
            end 
        end 
    end     
    
    if (hBS > dh) 
        kd = 18; 
    else 
        kd = 18 - 15 *  (hBS - dh) / dh; 
    end 

    kf = -4 + 0.7 * (f / 925 - 1); 

    L1 = L11 + ka + kd * log10(d) + kf * log10(f) - 9 * log10(b);
    
    L2 = -16.9 - 10 * log10(w) + 10 * log10(f) + 20 * log10(dh - hms) - 8 + 0.265 * fi; 
    
    if (L1 + L2 > 0) 
        Lnlos = L0 + L1 + L2; 
    else 
        Lnlos = L0; 
    end 
end
