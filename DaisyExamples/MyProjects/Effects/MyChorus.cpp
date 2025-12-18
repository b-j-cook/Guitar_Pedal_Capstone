#include "MyChorus.h"
#include <vector>
#include <cmath>

#define PI 3.1415926535

using namespace daisysp;

void MyChorus::Init(float rate, float depth_ms, float base_ms, float wet, float fs, int numVoices)
{
    this->baseDelay = base_ms * 1e-3 * fs;
    this->modDepth = depth_ms * 1e-3 *fs;
    this->rates.resize(numVoices);
    this->phases.resize(numVoices);
    this->numVoices = numVoices;
    int i;
    for (i = 0; i < numVoices; ++i) {
        this->rates.at(i) = rate * ((0.2 * i / (numVoices - 1)) + 0.9);
    }
    for (i = 0; i < numVoices; ++i) {
        this->phases.at(i) = 2 * PI * i / numVoices;
    }
    this->maxDelay = ceil(this->baseDelay + this->modDepth) + 3;
    this->buffer.assign(maxDelay, 0);
    this->writeIdx = 1;
    this->fs = fs;
    this->wet = wet;
}

float MyChorus::Process(float in)
{
    buffer[writeIdx] = in;

    float wetSample = 0.0f;

    for(int v = 0; v < numVoices; v++)
    {
        float m = sinf(2.0f * PI * rates[v] * float(writeIdx) / fs + phases[v]);
        float d = baseDelay + modDepth * m;
        float readPos = float(writeIdx) - d;

        while(readPos < 0)
            readPos += maxDelay;

        int k = int(floorf(readPos));
        float a = readPos - k;

        int idxm1 = (k - 1 + maxDelay) % maxDelay;
        int idx0  = k;
        int idx1  = (k + 1) % maxDelay;
        int idx2  = (k + 2) % maxDelay;

        float gm1 = buffer[idxm1];
        float g0  = buffer[idx0];
        float g1  = buffer[idx1];
        float g2  = buffer[idx2];

        float Lm1 = (-a*a*a + 3*a*a - 2*a) / 6.0f;
        float L0  = (0.5f*a*a*a - a*a - 0.5f*a + 1.0f);
        float L1  = (-0.5f*a*a*a + 0.5f*a*a + a);
        float L2  = (a*a*a - a) / 6.0f;

        wetSample += (Lm1*gm1 + L0*g0 + L1*g1 + L2*g2);
    }

    wetSample /= float(numVoices);

    float out = (1.0f - wet)*in + wet*wetSample;

    writeIdx++;
    if(writeIdx >= maxDelay)
        writeIdx = 0;

    return out;
}