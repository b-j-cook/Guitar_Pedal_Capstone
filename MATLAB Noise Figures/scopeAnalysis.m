% loadNoiseFloor.m
filename = "SineWaveRCDivider600mV300Hz.csv";

% Check file exists
if ~isfile(filename)
    error("File not found: %s", filename)
end

% Create import options (lets MATLAB detect delimiter, variable types, headers)
opts = detectImportOptions(filename);

% skips all the header info
opts.VariableNamesLine = 21;
opts.DataLines = [22 Inf];

% Read into a table
T = readtable(filename, opts);

% Display a preview
disp(T(1:min(10,height(T)), :))

t = T.TIME;
v = T.CH2;

%% FFT
dt = mean(diff(t));
Fs = 1/dt;
N = length(v); % Number of data points
f = (0:N-1)*(Fs/N); % Frequency vector
V = fft(v); % Compute the FFT of the signal

% Plot time-domain signal (optional)
figure;
subplot(2,1,1);
plot(t, v);
xlabel('Time (s)');
ylabel('Pedal Output (V)');
title('Time Domain');

% Plot single-sided amplitude spectrum
subplot(2,1,2);
semilogx(f, abs(V), 'LineWidth', 1.2);
xlabel('Frequency (Hz)');
ylabel('|V(f)|');
title('Spectrum');
grid on;
xlim([0 Fs]);

sgtitle("Time and Frequency Domains for Noise Floor")