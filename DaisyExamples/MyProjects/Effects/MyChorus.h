#pragma once
#ifndef MYCHORUS_H
#define MYCHORUS_H
#include <vector>
#include "daisysp.h"

namespace daisysp
{
    class MyChorus
    {
        public:
            void Init(float rate, float depth_ms, float base_ms, float wet, float fs, int numVoices);
            float Process(float in);
        private:
            float baseDelay, modDepth, fs, wet;
            std::vector<float> rates, phases, buffer;
            int numVoices, maxDelay, writeIdx;
    };
}

#endif