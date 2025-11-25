#include "MyChorus.h"
#include <vector>
#include <cmath>

#define PI 3.1415926535

using namespace daisysp;

void MyChorus::Init(float rate, float depth_ms, float base_ms, float wet, float fs, int numVoices)
{
    this->baseDelay = base_ms * 1e-3 * fs;
    this->modDepth = depth_ms * 1e-3 *fs;
    this->rates.reserve(numVoices);
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

float MyChorus::Process(float in, int i)
{
    this->buffer.at(i) = in;
    float wetSample = 0;
    for (int v = 0; v < this->numVoices; ++v) {
        float m = sin(2 * PI * this->rates.at(v) * i / this->fs + this->phases.at(v));
        float d = this->baseDelay + this->modDepth * m;
        float readPos = this->writeIdx - d;
        if (readPos < 1) {
            readPos += this->maxDelay;
        }
        int k = floor(readPos);
        float alpha = readPos - k;
        int idxm1 = (k - 2) % this->maxDelay + 1;
        int idx0 = (k - 1) % this->maxDelay + 1;
        int idx1 = k % this->maxDelay + 1;
        int idx2 = (k + 1) % this->maxDelay + 1;
        float gm1 = this->buffer.at(idxm1);
        float g0 = this->buffer.at(idx0);
        float g1 = this->buffer.at(idx1);
        float g2 = this->buffer.at(idx2);
        float Lm1 = (-1.0f*pow(alpha, 3) + 3.0f*pow(alpha, 2) - 2.0f*alpha) / 6.0f;
        float L0 = 0.5f*pow(alpha, 3) - pow(alpha, 2) - 0.5f*alpha + 1.0f;
        float L1 = -0.5f*pow(alpha, 3) + 0.5f*pow(alpha, 2) + alpha;
        float L2 = (pow(alpha, 3) - alpha) / 6.0f;
        wetSample += (Lm1*gm1 + L0*g0 + L1*g1 + L2*g2);
    }
    wetSample /= this->numVoices;
    float out = (1 - wet)*in + wet*wetSample;
    this->writeIdx++;
    if (this->writeIdx > this->maxDelay) {
        this->writeIdx = 1;
    }
    return out;
}