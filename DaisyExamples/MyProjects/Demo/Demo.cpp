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
FIRTaps taps;
FIR<NUMTAPS, 2> inputFilter;
float firCoeffs[NUMTAPS];
MyTremolo trem;
MyChorus chorus;

static bool tremOn;
static bool chorOn;

void AudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{	
		float inL = inputFilter.Process(in[0][i]);
		float inR = inputFilter.Process(in[1][i]);
		if (tremOn) {
			inL = trem.Process(inL);
			inR = trem.Process(inR);
		}
		if (chorOn) {
			out[0][i] = chorus.Process(inL);
			out[1][i] = chorus.Process(inR);
		}
		out[0][i] = inL;
		out[1][i] = inR;
	}
}

int main(void)
{
	hw.Init();
	hw.SetAudioBlockSize(2); // number of samples handled per callback
	hw.SetAudioSampleRate(SaiHandle::Config::SampleRate::SAI_48KHZ);
	hw.StartAudio(AudioCallback);
	hw.Configure();
	taps.Init(NUMTAPS);
	taps.MakeLowPassFIR(firCoeffs, 8000.0f, hw.AudioSampleRate());
	inputFilter.Init(firCoeffs, NUMTAPS, false);
	trem.Init(0.5f, 5.0f, hw.AudioSampleRate());
	chorus.Init(0.33f, 6.0f, 12.0f, 0.4f, hw.AudioSampleRate(), 3);

	tremOn = false;
	chorOn = false;

	GPIO trem_button;
	GPIO chor_button;

	my_button.Init(D15, GPIO::Mode::INPUT, GPIO::Pull::PULLUP);
	my_button.Init(D16, GPIO::Mode::INPUT, GPIO::Pull::PULLUP);

	for(;;)
    {
		bool tremPushed = !trem_button.Read();
		bool chorPushed = !chor_button.Read();
		if (tremPushed) {
			tremOn = !tremOn;
			System::Delay(1000);
		}
		if (chorPushed) {
			chorOn = !chorOn;
			System::Delay(1000);
		}
    }
}
