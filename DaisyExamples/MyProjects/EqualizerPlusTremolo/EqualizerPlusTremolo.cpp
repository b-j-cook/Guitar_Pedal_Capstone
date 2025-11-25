#include "daisy_seed.h"
#include "daisysp.h"
#include <cmath>
#include "../lib/biquad.h"
#include "../lib/FIRTaps.h"
#include "../Effects/Tremolo.h"
#include "../MyProjects/Effects/Equalizer.h"
#include "../MyProjects/Effects/Chorus.h"

using namespace daisy;
using namespace daisysp;
using namespace daisy::seed;

#define NUM_TAPS 101
#define PI 3.1415926535
FIR<NUM_TAPS, 4> inputFilter;
float firCoeffs[NUM_TAPS];
FIRTaps fir;
Tremolo trem;
Equalizer eq;
Chorus chorus;

DaisySeed hw;
Oscillator lfo;
CpuLoadMeter loadMeter;
float depth = 0.5f;
float rate = 5.0f;

static float tempL[64];
static float tempR[64];

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	loadMeter.OnBlockStart();
	float maxabs = 0.0f;
	for (size_t i = 0; i < size; i++)
	{
		float inL = inputFilter.Process(in[0][i]);
		float inR = inputFilter.Process(in[1][i]);
		float samp = lfo.Process();
		float tremL = trem.Process(inL, samp);
		float tremR = trem.Process(inR, samp); 
		float absVal = eq.Process(tremL, tremR, tempL, tempR, i);
		if(absVal > maxabs) {
			maxabs = absVal;
		}
	}
	float normGain = eq.NormGain(maxabs, 0.9f);
	for(size_t i = 0; i < size; i++) {
		out[0][i] = tempL[i] * normGain;
		out[1][i] = tempR[i] * normGain;
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
	fir.Init(NUM_TAPS);
	fir.MakeBandPassFIR(firCoeffs, 80, 6000, hw.AudioSampleRate());
	inputFilter.Init(firCoeffs, NUM_TAPS, false);
	trem.Init(depth);
	chorus.Init()
	eq.Init(300.0f, 6.0f, 1.0f, hw.AudioSampleRate());
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