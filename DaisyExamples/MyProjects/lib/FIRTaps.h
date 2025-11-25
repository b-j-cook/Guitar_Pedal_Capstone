#pragma once
#ifndef DSY_FIRTAPS_H
#define DSY_FIRTAPS_H

namespace daisysp
{

class FIRTaps
{
    public:
        FIRTaps() {};
        ~FIRTaps() {};
        void Init(int numTaps);
        void MakeLowPassFIR(float *coeffs, float cutoffFreq, float sampRate);
        void MakeBandPassFIR(float *coeffs, float lowCut, float highCut, float sampRate);
    private:
        int numTaps;
};
}
#endif