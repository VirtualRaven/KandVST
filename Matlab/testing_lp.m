fs = 48000;
fc = 12000;
w = 2*pi*fs;
wc = 2*pi*fc;

%% LP
[b, a] = butter(2, fc/(fs/2), 'low')
figure
freqz(b,a, 256, fs);
title('(LP) Genererad')

wa = 2*fs*tan(wc/(2*fs));

a0 = 4*(fs^2)+2*fs*wa*sqrt(2)+(wa^2);

b2 = (wa^2)/a0;
b1 = (2*wa^2)/a0;
b0 = (wa^2)/a0;

btest = [b0, b1, b2];

a2 = ((wa^2)-2*sqrt(2)*wa*fs + 4*(fs^2))/a0;
a1 = (2*(wa^2)-8*(fs^2))/a0;

atest = [1, a1, a2];

K = sum(atest)/sum(btest);
btest = K*btest;

figure
freqz(btest, atest, 256, fs)
title('(LP) Beräknad')

%% HP

[b, a] = butter(2, fc/(fs/2), 'high')
figure
freqz(b,a, 256, fs);
title('(HP) Genererad')

wa = 2*fs*tan(wc/(2*fs));
a0 = 4*(fs^2)+2*fs*wa*sqrt(2)+(wa^2);
b2 = (4*fs^2)/a0;
b1 = (-8*fs^2)/a0;
b0 = (4*fs^2)/a0;

btest = [b0, b1, b2]

a2 = ((wa^2)-2*sqrt(2)*wa*fs + 4*(fs^2))/a0;
a1 = (2*(wa^2)-8*(fs^2))/a0;

atest = [1, a1, a2];
K = sum(atest)/sum(btest);
%btest = K*btest;

figure
freqz(btest, atest, 256, fs)
title('(HP) Beräknad')
