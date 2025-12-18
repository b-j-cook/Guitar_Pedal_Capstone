#include "daisy_seed.h"
#include "daisysp.h"
#include "../Effects/MyTremolo.h"

using namespace daisy;
using namespace daisysp;
using namespace daisy::seed;

DaisySeed hw;
// MyTremolo trem;

float gain = 3.0;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		out[0][i] = in[0][i]*gain;
		out[1][i] = in[1][i]*gain;
		// out[0][i] = trem.Process(in[0][i])*gain;
		// out[1][i] = trem.Process(in[1][i])*gain;
	}
}

int main(void)
{
	hw.Init();
	hw.SetAudioBlockSize(4); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_96KHZ);
	hw.StartAudio(AudioCallback);
	// trem.Init(0.5f, 5.0f, hw.AudioSampleRate());
	while(1) {}
}
