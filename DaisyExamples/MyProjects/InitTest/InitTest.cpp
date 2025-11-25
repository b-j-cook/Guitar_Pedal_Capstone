#include "daisy_seed.h"
#include "daisysp.h"
#include "../Effects/MyTremolo.h"

using namespace daisy;
using namespace daisysp;
using namespace daisy::seed;

DaisySeed hw;
// MyTremolo trem;
Oscillator lfo;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{	
		// out[0][i] = trem.Process(in[0][i]);
		// out[1][i] = trem.Process(in[1][i]);
		out[0][i] = lfo.Process();
		out[1][i] = lfo.Process();
	}
}

int main(void)
{
	hw.Init();
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.StartAudio(AudioCallback);
	hw.Configure();
	// trem.Init(0.5f, 5.0f, hw.AudioSampleRate());
	lfo.Init(hw.AudioSampleRate());
	lfo.SetWaveform(Oscillator::WAVE_SIN);
	lfo.SetAmp(0.2f);
	lfo.SetFreq(300.0f);
	// bool led_state = true;
    // while(1)
    // {
	// 	led_state = !led_state;
    //     hw.SetLed(!led_state);
	// 	System::Delay(5000);
    // }
}
