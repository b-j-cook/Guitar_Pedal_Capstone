#include "daisy_seed.h"
#include "daisysp.h"
#include "../Effects/MyTremolo.h"
#include "../lib/FIRTaps.h"
#include "../Effects/MyChorus.h"

using namespace daisy;
using namespace daisysp;
using namespace daisy::seed;

#define NUMTAPS 101

DaisySeed hw;
// FIRTaps taps;
// FIR<NUMTAPS, 2> inputFilter;
// float firCoeffs[NUMTAPS];
MyTremolo trem;
// Oscillator lfo;
// MyChorus chorus;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{	
		// float inL = inputFilter.Process(in[0][i]);
		// float inR = inputFilter.Process(in[1][i]);
		out[0][i] = trem.Process(in[0][i]);
		out[1][i] = trem.Process(in[1][i]);
		// out[0][i] = chorus.Process(in[0][i]);
		// out[1][i] = chorus.Process(in[1][i]);
		// out[0][i] = in[0][i];
		// out[1][i] = in[1][i];
		// float samp = lfo.Process();
		// float trem = (1-1.0f/2)+1.0f/2*samp;
		// out[0][i] = in[0][i] * trem;
		// out[1][i] = in[1][i] * trem;
	}
}

int main(void)
{
	hw.Init();
	hw.SetAudioBlockSize(2); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.StartAudio(AudioCallback);
	hw.Configure();
	// taps.Init(NUMTAPS);
	// taps.MakeLowPassFIR(firCoeffs, 8000.0f, hw.AudioSampleRate());
	// inputFilter.Init(firCoeffs, NUMTAPS, false);
	trem.Init(0.5f, 5.0f, hw.AudioSampleRate());
	// chorus.Init(0.33f, 6.0f, 12.0f, 0.4f, hw.AudioSampleRate(), 3);
	// lfo.Init(hw.AudioSampleRate());
	// lfo.SetWaveform(Oscillator::WAVE_SIN);
	// lfo.SetAmp(0.2f);
	// lfo.SetFreq(7.0f);
	// bool led_state = true;
    // while(1)
    // {
	// 	led_state = !led_state;
    //     hw.SetLed(!led_state);
	// 	System::Delay(5000);
    // }
}
