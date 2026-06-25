#pragma once

#include "SoundTouch.h"
#include <memory>

class PitchShifter
{
public:
    PitchShifter();
    ~PitchShifter();

    void prepare(double sampleRate);
    void processPitchShift(float* audioData, int numSamples, float shiftRatio, float correctionAmount);

private:
    std::unique_ptr<soundtouch::SoundTouch> soundTouch;
    double sampleRate = 44100.0;
};