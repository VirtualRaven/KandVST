% Fourier series representation of basic synth wave forms



% Main constants
Fs = 100; % Hz Sampling frequency
N  = 1000; % Number of samples to be generated
K  =  100; % Number of fourier coeff to calculate


% Wave constants
F  = 2*Fs/N; % Hz target wave from frequency
A  = 1; % Target wave amplitude

T = 0:1/Fs:N/Fs;


%%
%                  ____      ____
% Square duty _____|  |______|  |_____
%
% See note 1.1

SqDuty = 0.9; % Duty cycle

 
% Lambda functions for Ak anb Bk when k ~= 0
sqdAk = @(k) -A* sin(2*pi*SqDuty*k)   /(pi*k);
sqdBk = @(k)  A*(cos(2*pi*SqDuty*k)-1)/(pi*k);
sqd = synthesize(sqdAk,sqdBk,K,F,T)+ A*(1-SqDuty);


%%
%                __     __
% Square     ___|  |___|  |___
%
% See note 1.1
 
sqAk = @(k) 0;
sqBk = @(k)  -2*A/(pi*k)*mod(k,2);
sq = synthesize(sqAk,sqBk,K,F,T)+ A/2;



%%
%          /| /|
% Ramp   |/ |/ |
% See note 1.2

rampAk = @(k) 0;
rampBk = @(k) -A/(pi*k);
ramp = synthesize(rampAk,rampBk,K,F,T)+A/2;

%%
%        |\ |\ |
% Ramp   | \| \|
% See note 1.3

rampiAk = @(k) 0;
rampiBk = @(k) A/(pi*k);
rampi = synthesize(rampiAk,rampiBk,K,F,T)+A/2;

%%
%           /\  /
% Triangle /  \/
% See note 1.4
triAk = @(k) -4*A/(pi*k)^2 * mod(k,2);
triBk = @(k) 0;
tri = synthesize(triAk,triBk,K,F,T)+A/2;

%% All

figure('Name', 'Wave forms')
subplot(2,3,1);
plot(T,sin(2*pi*F*T));
title('Sine')

subplot(2,3,2);
plot(T,sqd);
title('Square duty');

subplot(2,3,3);
plot(T,sq);
title('Squre');

subplot(2,3,4);
plot(T,ramp);
title('Ramp');

subplot(2,3,5);
plot(T,rampi);
title('Inverse Ramp');

subplot(2,3,6);
plot(T,tri);
title('Triangle')