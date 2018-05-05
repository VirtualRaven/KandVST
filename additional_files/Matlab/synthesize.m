function [ func ] = synthesize( ak, bk, k,f ,T)
%synthesize Synthesize fourie series based on trigometric coefficients
    omega = 2*pi*f;
    s = size(T);
    func = repelem(0,s(2)) ; 
    
    for i = 1:k
       func = func + ak(i)*cos(omega*i*T) + bk(i)*sin(omega*i*T);
    end
    

end

