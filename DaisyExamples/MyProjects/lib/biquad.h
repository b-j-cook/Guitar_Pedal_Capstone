#pragma once
#ifndef DSY_BIQUAD_H
#define DSY_BIQUAD_H

namespace daisysp
{
class Biquad
{
    public:
        Biquad() {}
        ~Biquad() {}
        void Init();
        void SetPeaking(float freq, float gain, float Q, float samp_rate);
        float Process(float in);
    private:
        float a0, a1, a2;
        float b0, b1, b2;
        float z1, z2;
};
}
#endif