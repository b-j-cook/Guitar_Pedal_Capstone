bandwidth = 400;
fs = 48000;
r = exp(-1*(pi*bandwidth/fs));
disp("r: "+r);

fo = 300;
wo = (fo*2*pi)/fs;
disp("wo: "+wo);

% b = [1-r, 0, r-1];
% a = [1/2, -1*r*cos(wo), 1/2*r^2];

a = [1, -1.947297559760623686386793451674748212099, 0.949455380439758189758947537484345957637];
b = [0.03715808689594268798828125, 0, -0.03715808689594268798828125];

a = single(a);
b = single(b);

[H, w] = freqz(b, a, 8192, fs);

figure;
zplane(b, a);
title('Equalizer Pole-Zero Plot');

figure;
yyaxis left;
plot(w, 20*log10(abs(H)));
hold on;
xlabel('Frequency (Hz)');
ylabel('Magnitude (dB)');
title('Equalizer Bode Plot');
grid on;

yyaxis right;
plot(w, angle(H)*180/pi);
ylabel('Phase (degrees)');
xlim([0 8000]);
hold off;

N = 5 * fs;
x = zeros(N, 1);

y = filter(b, a, x);

noise_rms = sqrt(mean(y.^2));
noise_dbfs = 20*log10(noise_rms + eps);

disp("Noise RMS: " + noise_rms);
disp("Noise level (dBFS): " + noise_dbfs);