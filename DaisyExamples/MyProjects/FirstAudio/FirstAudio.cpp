#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

DaisySeed hw;
Oscillator osc;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{	
		float sine_signal = osc.Process();
		out[0][i] = in[0][i]*sine_signal;
		out[1][i] = in[0][i]*sine_signal;
	}
}

int main(void)
{
	hw.Init();
	osc.Init(hw.AudioSampleRate());
	hw.StartAudio(AudioCallback);
	while(1) {}
}

// to upload, use make program-dfu