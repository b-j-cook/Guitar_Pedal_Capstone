#include "Equalizer.h"
#include <cmath>

using namespace daisysp;

void Equalizer::Init(float fc, float G, float Q, float fs)
{
    this->fc = fc;
    this->G = G;
    this->Q = Q;
    this->fs = fs;
    this->eqFiltL.Init();
	this->eqFiltR.Init();
	this->eqFiltL.SetPeaking(this->fc, this->G, this->Q, this->fs);
	eqFiltR.SetPeaking(this->fc, this->G, this->Q, this->fs);
}

float Equalizer::Process(float sampL, float sampR, float *tempL[64], float *tempR[64], int i)
{
    float eqOutL = eqFiltL.Process(sampL);
    *tempL[i] = eqOutL;
    float eqOutR = eqFiltR.Process(sampR);
    *tempR[i] = eqOutR;
    float absVal = fmaxf(fabsf(eqOutL), fabsf(eqOutR));
    return absVal;
}

float Equalizer::NormGain(float maxabs, float normTo)
{
    return (maxabs > 1e-6f) ? normTo/maxabs : 1.0f;
}