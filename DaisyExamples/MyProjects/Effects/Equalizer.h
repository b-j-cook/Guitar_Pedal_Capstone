#pragma once
#ifndef EQUALIZER_H
#define EQUALIZER_H
#include "../MyProjects/lib/biquad.h"

namespace daisysp
{
    class Equalizer
    {
        public:
            void Init(float fc, float G, float Q, float fs);
            float Process(float sampL, float sampR, float *tempL[64], float *tempR[64], int i);
            float NormGain(float maxabs, float normTo);
        private:
            Biquad eqFiltL, eqFiltR;
            float fc, G, Q, fs;
    };
}
#endif