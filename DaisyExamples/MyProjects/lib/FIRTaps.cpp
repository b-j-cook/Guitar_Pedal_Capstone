#include "FIRTaps.h"
#include <cmath>
#include <vector>

#define PI 3.1415926535

using namespace daisysp;

void FIRTaps::Init(int numTaps) {
    this->numTaps = numTaps;
}

void FIRTaps::MakeLowPassFIR(float *coeffs, float cutoffFreq, float sampRate) {
	float fcp = cutoffFreq/sampRate;
	int M = this->numTaps-1;
	for(int i=0; i<this->numTaps; i++) {
		if(i == M/2) {
			coeffs[i] = 2.0f*fcp;
		} else {
			float x = (float)(i - M/2);
			coeffs[i] = sinf(2.0f * PI * fcp * x) / (PI * x);
		}
		coeffs[i]*=0.54f-0.46f*cosf(2.0f*PI*i/M);
	}
} // used normalized sinc function for taps and hamming window function to reduce ripples

void FIRTaps::MakeBandPassFIR(float *coeffs, float lowCut, float highCut, float sampRate) {
	std::vector<float> lpLow(numTaps);
    std::vector<float> lpHigh(numTaps);
	MakeLowPassFIR(lpLow.data(), lowCut, sampRate);
	MakeLowPassFIR(lpHigh.data(), highCut, sampRate);
	for(int i=0; i<this->numTaps; i++) {
		coeffs[i] = lpLow[i] - lpHigh[i];
	}
}