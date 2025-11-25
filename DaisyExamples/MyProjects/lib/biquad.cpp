#include <math.h>
#include "biquad.h"

using namespace daisysp;

#define PI 3.1415926535

void Biquad::Init()
{
    z1 = 0.0f;
    z2 = 0.0f;
    a0 = 1.0f;
    a1 = 0.0f;
    a2 = 0.0f;
    b0 = 1.0f;
    b1 = 0.0f;
    b2 = 0.0f;
}

void Biquad::SetPeaking(float freq, float gain, float Q, float samp_rate)
{
    const float A = powf(10.0f, gain/40.0f);
    const float w0 = 2.0f*PI*freq/samp_rate;
    const float alpha = sinf(w0)/(2.0f*Q);
    const float cosw0 = cosf(w0);
    b0 = 1.0f+alpha*A;
    b1 = -2.0f*cosw0;
    b2 = 1.0f-alpha*A;
    a0 = 1.0f+alpha/A;
    a1 = -2.0f*cosw0;
    a2 = 1.0f-alpha/A;
    b0 /= a0;
    b1 /= a0;
    b2 /= a0;
}

float Biquad::Process(float in)
{
    float out = b0*in+z1;
    z1 = b1*in-a1*out+z2;
    z2 = b2*in-a2*out;
    return out;
}