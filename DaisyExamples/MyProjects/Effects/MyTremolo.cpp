#include "MyTremolo.h"

using namespace daisysp;

void MyTremolo::Init(float depth, float rate, float fs)
{
    this->depth = depth;
    this->rate = rate;
    this->fs = fs;
    this->lfo.Init(this->fs);
	this->lfo.SetWaveform(Oscillator::WAVE_SIN);
	this->lfo.SetAmp(1.0f);
	this->lfo.SetFreq(this->rate);
}

float MyTremolo::Process(float in)
{
    float samp = this->lfo.Process();
    float effect = (1-this->depth/2)+this->depth/2*samp;
    return in*effect;
}