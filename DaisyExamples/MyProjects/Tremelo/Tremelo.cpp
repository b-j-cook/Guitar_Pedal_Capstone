#include "daisy_seed.h"
#include "daisysp.h"
#include <cmath>

using namespace daisy;
using namespace daisysp;

#define NUM_TAPS 101
#define PI 3.1415926535
FIR<NUM_TAPS, 4> effectFilter;
float firCoeffs[NUM_TAPS];

void MakeLowPassFIR(float *coeffs, int numTaps, float cutoffFreq, float sampRate) {
	float fcp = cutoffFreq/sampRate;
	int M = numTaps-1;
	for(int i=0; i<numTaps; i++) {
		if(i == M/2) {
			coeffs[i] = 2.0f*fcp;
		} else {
			float x = (float)(i - M/2);
			coeffs[i] = sinf(2.0f * PI * fcp * x) / (PI * x);
		}
		coeffs[i]*=0.54f-0.46f*cosf(2.0f*PI*i/M);
	}
} // used normalized sinc function for taps and hamming window function to reduce ripples

DaisySeed hw;
Oscillator lfo;
CpuLoadMeter loadMeter;
float depth = 0.5f;
float rate = 5.0f;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	loadMeter.OnBlockStart();
	for (size_t i = 0; i < size; i++)
	{
		float samp = lfo.Process();
		float raw_effect = (1-depth/2)+depth/2*samp;
		float effect = effectFilter.Process(raw_effect);
		out[0][i] = in[0][i]*effect;
		out[1][i] = in[1][i]*effect;
	}
	loadMeter.OnBlockEnd();
}

enum AdcChannel {
	DEPTH = 0,
	RATE,
	NUM_CHANNELS
};

int main(void)
{
	hw.Init();
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.StartLog();
	loadMeter.Init(hw.AudioSampleRate(), hw.AudioBlockSize());
	lfo.Init(hw.AudioSampleRate());
	AdcChannelConfig my_adc_config[NUM_CHANNELS];
	my_adc_config[DEPTH].InitSingle(A0);
	my_adc_config[RATE].InitSingle(A1);
	hw.adc.Init(my_adc_config, NUM_CHANNELS);
	lfo.SetWaveform(Oscillator::WAVE_SIN);
	lfo.SetAmp(1.0f);
	MakeLowPassFIR(firCoeffs, NUM_TAPS, 30.0f, hw.AudioSampleRate());
	effectFilter.Init(firCoeffs, NUM_TAPS, false);
	hw.StartAudio(AudioCallback);
	while(1) {
		rate = hw.adc.GetFloat(RATE)*10.0f;
		depth = hw.adc.GetFloat(DEPTH);
		lfo.SetFreq(rate);
		const float maxLoad = loadMeter.GetMaxCpuLoad();
		const float avgLoad = loadMeter.GetAvgCpuLoad();
		const float minLoad = loadMeter.GetMinCpuLoad();
		hw.PrintLine("Processing Load in %");
		hw.PrintLine("Max: " FLT_FMT3, FLT_VAR3(maxLoad*100.0f));
		hw.PrintLine("Avg: " FLT_FMT3, FLT_VAR3(avgLoad*100.0f));
		hw.PrintLine("Min: " FLT_FMT3, FLT_VAR3(minLoad*100.0f));
		System::Delay(500);
	}
}

// Equations found here: https://christianfloisand.wordpress.com/2012/04/18/coding-some-tremolo/