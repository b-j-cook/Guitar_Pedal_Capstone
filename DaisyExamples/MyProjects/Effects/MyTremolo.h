#pragma once
#ifndef MYTREMOLO_H
#define MYTREMOLO_H
#include "daisysp.h"

namespace daisysp
{
    class MyTremolo
    {
        public:
            MyTremolo() {};
            ~MyTremolo() {};
            void Init(float depth, float rate, float fs);
            float Process(float in);
        private:
            float depth, rate, fs;
            Oscillator lfo;
    };
}

#endif