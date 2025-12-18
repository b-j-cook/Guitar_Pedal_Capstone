[g, fs, t] = getInput("C Minor");
effect = "Equalizer";
soundOn = true;

depth = 0.7;
rate = 5;
wave = 2*pi*rate;

y = filter(g, fs, 101, 80, 6000);
[y, e] = tremolo(y, t, depth, wave);
y = chorus(y, fs, 0.25, 4, 25, 0.7);
y = equalizer(y, fs, 300, 1, 1);
y = sat_norm(y);
Test(g, y, t, fs, soundOn, effect, e);

function [g, fs, t] = getInput(input)
    switch input
        case "C Minor"
            [guitar, fs] = audioread("guitar-single-note-g_120bpm_C_minor.wav");
            if size(guitar, 2)>1, guitar = mean(guitar, 2); end
            g = guitar.';
        case "Lead Guitar"
            [guitar, fs] = audioread("minor-guitar-lead_75bpm_C_minor.wav");
            if size(guitar, 2)>1, guitar = mean(guitar, 2); end
            g = guitar.';
        case "Noise"
            fs = 96000;
            g = randn(1, fs);
        case "Sine"
            fs = 96000;
            duration = 2;
            f = 260;
            t = (0:1/fs:(duration - 1/fs))';
            g = sin(2*pi*f*t);
        otherwise
            disp("Improper Input");
            fs = 96000;
            g = zeros(1, fs);
    end
    if ~exist('t', 'var')
        t = (0:length(g)-1)' / fs;
    end
end

function g = filter(g_in, fs, N_fir, low, high)
    bg = fir1(N_fir, [low,high]/(fs/2));
    g = filtfilt(bg, 1, g_in);
end

function [y, e] = tremolo(g, t, depth, wave)
    g = g(:);
    t = t(:);
    e = 1-depth/2+depth/2*sin(wave.*t);
    y = e.*g;
end

function y = chorus(g, fs, rate, depth_ms, base_ms, wet)
    N = length(g);
    y = zeros(N, 1);

    baseDelay = base_ms * 1e-3 * fs;
    modDepth  = depth_ms * 1e-3 * fs;

    numVoices = 3;
    rates = rate * [0.9, 1.0, 1.1];
    phases = 2 * pi * [0, 1/3, 2/3];

    maxDelay = ceil(baseDelay + modDepth) + 3;
    buffer = zeros(maxDelay, 1);
    writeIdx = 1;

    for n = 1:N
        buffer(writeIdx) = g(n);
        wetSample = 0;

        for v = 1:numVoices
            m = sin(2 * pi * rates(v) * n / fs + phases(v));
            d = baseDelay + modDepth * m;
            readPos = writeIdx - d;
            if readPos < 1
                readPos = readPos + maxDelay;
            end

            k = floor(readPos);
            alpha = readPos - k;
            idxm1 = mod(k - 2, maxDelay) + 1;
            idx0  = mod(k - 1, maxDelay) + 1;
            idx1  = mod(k, maxDelay) + 1;
            idx2  = mod(k + 1, maxDelay) + 1;

            gm1 = buffer(idxm1);
            g0  = buffer(idx0);
            g1  = buffer(idx1);
            g2  = buffer(idx2);

            Lm1 = (-alpha^3 + 3*alpha^2 - 2*alpha) / 6;
            L0  = (0.5*alpha^3 - alpha^2 - 0.5*alpha + 1);
            L1  = (-0.5*alpha^3 + 0.5*alpha^2 + alpha);
            L2  = (alpha^3 - alpha) / 6;

            wetSample = wetSample + (Lm1*gm1 + L0*g0 + L1*g1 + L2*g2);
        end

        wetSample = wetSample / numVoices;

        y(n) = (1 - wet)*g(n) + wet*wetSample;

        writeIdx = writeIdx + 1;
        if writeIdx > maxDelay
            writeIdx = 1;
        end
    end

    y = y / max(abs(y) + eps);
end

function y = equalizer(y_in, fs, f0, gain, Q)
    w0 = 2*pi*f0/fs;
    alpha = sin(w0)/(2*Q);
    A = 10^(gain/40);
    b = [1 + alpha*A, -2*cos(w0), 1 - alpha*A];
    a = [1 + alpha/A, -2*cos(w0), 1 - alpha/A];
    y = filtfilt(b/a(1), a/a(1), y_in);
end

function y = equalizer2(y_in, fs, f0, gain, Q)
    w0 = 2*pi*f0/fs;
    bandwidth = Q*fs;
    r = exp(-1*(pi*bandwidth/fs));
    b = [1, 0, -1];
    a = [1, -2*r*cos(w0), r^2];
    y = filtfilt(b, a, y_in);
end

function y = equalizer3(y_in, fs, f0, gain, Q)
    a = [1, -1.94876194000244140625, 0.94945538043975830078125];
    b = [0.03715808689594268798828125, 0, -0.03715808689594268798828125];
    a = single(a);
    b = single(b);
    y = filtfilt(b, a, y_in);
end

function y = sat_norm(y_in)
    y_sat = tanh(1.3*y_in);
    y = y_sat/max(abs(y_sat))*0.9;
end

function Test(g, y, t, fs, soundOn, effect, e)
    %% Plotting
    figure;
    plot(t, y, 'r'); 
    hold on;
    plot(t, g, "g--");
    xlabel("Time (s)");
    ylabel("Amplitude");
    title(effect+" Output and Envelope");
    legend("Output", "Input");
    
    %% Play Sounds
    if soundOn
        % disp("Playing clean guitar output...");
        % soundsc(g, fs);
        % audiowrite('clean_output.wav', g, fs);
        % pause(length(g)/fs+0.5);
        disp("Playing post effect output...");
        sound(y, fs);
        audiowrite('effect_output.wav', y, fs);
        pause(length(y)/fs+0.5);
    end

    %% Test (for sine wave really)
    % Modulation Accuracy
    if effect == "Tremolo"
        y_env = abs(hilbert(y));
        corr = corrcoef(y_env, e);
        disp("Envelope correlation: "+corr(1,2));
    
        % THD
        thd_val = thd(y, fs);
        disp("THD (dB): "+thd_val);
    
        % Noise Floor
        y_sil = e.*zeros(size(g));
        disp("Noise RMS: "+rms(y_sil));
    end
    % Frequency Response
    switch(effect)
        case "Tremolo"
            white = randn(size(t));
            y_white = e.*white;
            [Pxx, f] = pwelch(y_white, [], [], [], fs);
            figure;
            plot(f, 10*log10(Pxx));
            title("Spectrum of Modulated Noise");
            xlabel("Frequency (Hz)");
            ylabel("Power Spectral Density (dB/Hz)");
        case "Equalizer"
            % Setting up filter parameters and sampling frequency
            fs = 96000;
            f0 = 300;
            Q = 1;
            gain = 1;
            
            % Calculating biquad filter coefficients (Peaking EQ)
            w0 = 2*pi*f0/fs;
            alpha = sin(w0)/(2*Q);
            A = 10^(gain/40);
            b = [1 + alpha*A, -2*cos(w0), 1 - alpha*A];
            a = [1 + alpha/A, -2*cos(w0), 1 - alpha/A];
            
            % Normalizing coefficients so a(1) = 1
            b = b / a(1);
            a = a / a(1);
            
            % Calculating the Frequency Response
            [H, w] = freqz(b, a, 4096, fs);
            
            % Pole-Zero Plot
            figure;
            zplane(b, a);
            title('Equalizer Pole-Zero Plot');
            
            % Bode Plot (Magnitude and Phase)
            figure;
            
            % Magnitude Plot (Top Subplot)
            subplot(2, 1, 1);
            plot(w, 20*log10(abs(H)));
            xlabel('Frequency (Hz)');
            ylabel('Magnitude (dB)');
            title('Equalizer Bode Plot (Magnitude)');
            xlim([0 8000]);
            grid on;
            
            % Phase Plot (Bottom Subplot)
            subplot(2, 1, 2);
            plot(w, angle(H)*180/pi);
            xlabel('Frequency (Hz)');
            ylabel('Phase (degrees)');
            title('Equalizer Bode Plot (Phase)');
            xlim([0 8000]);
            grid on;
        case "Chorus"
            disp("Generating Chorus analysis plots...");
    
            % Time-Domain (Zoomed 20 ms window)
            figure;
            winlen = round(0.02*fs);
            winstart = round(length(g)*0.3);
            idx = winstart:(winstart+winlen-1);
            
            plot(t(idx), g(idx), 'g--', 'LineWidth', 1.0); hold on;
            plot(t(idx), y(idx), 'r', 'LineWidth', 1.0);
            xlabel("Time (s)");
            ylabel("Amplitude");
            legend("Input", "Output");
            title("Chorus Effect - Zoomed Time Window (20 ms)");
            grid on;
    
            % Spectrograms (Input vs Output)
            figure;
            subplot(2,1,1);
            spectrogram(g, 1024, 768, 2048, fs, 'yaxis');
            title("Input Spectrogram");
            
            subplot(2,1,2);
            spectrogram(y, 1024, 768, 2048, fs, 'yaxis');
            title("Output Spectrogram");
    
            % Magnitude Spectrum Comparison
            Nfft = 65536;
            G = fft(g, Nfft);
            Y = fft(y, Nfft);
            f = (0:Nfft-1)/Nfft*fs;
    
            figure;
            plot(f(1:Nfft/2), 20*log10(abs(G(1:Nfft/2)) + eps), "g--"); hold on;
            plot(f(1:Nfft/2), 20*log10(abs(Y(1:Nfft/2)) + eps), "r");
            xlabel("Frequency (Hz)");
            ylabel("Magnitude (dB)");
            legend("Input","Output");
            title("Chorus Effect - Magnitude Spectrum");
            xlim([0 fs/2]);
            grid on;
    
            % Delay Modulation (LFO) Per Voice
            % Use chorus parameters from your main code
            rate = 0.25;
            depth_ms = 4;
            base_ms = 25;
            numVoices = 3;
            rates = rate * [0.9, 1.0, 1.1];
            phases = 2*pi*[0, 1/3, 2/3];
    
            n = (0:length(g)-1)';
            baseDelay = base_ms * 1e-3 * fs;
            modDepth  = depth_ms * 1e-3 * fs;
    
            figure;
            for v = 1:numVoices
                m = sin(2*pi*rates(v)*n/fs + phases(v));
                d = (baseDelay + modDepth*m) / fs * 1000;
    
                subplot(numVoices,1,v);
                plot(n/fs, d);
                ylabel(sprintf("Delay %d (ms)", v));
                if v == 1
                    title("Chorus Effect - Instantaneous Delay (LFOs)");
                end
                grid on;
            end
            xlabel("Time (s)");
    end
end