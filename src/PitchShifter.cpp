#include "PitchShifter.h"
#include <algorithm>

PitchShifter::PitchShifter()
{
    soundTouch = std::make_unique<soundtouch::SoundTouch>();
}

PitchShifter::~PitchShifter() {}

void PitchShifter::prepare(double newSampleRate)
{
    sampleRate = newSampleRate;
    soundTouch->setSampleRate((uint32_t)sampleRate);
    soundTouch->setChannels(1);
    soundTouch->setTempoChange(0.0f);
    soundTouch->setPitchSemiTones(0.0f);
}

void PitchShifter::processPitchShift(float* audioData, int numSamples, float shiftRatio, float correctionAmount)
{
    if (shiftRatio <= 0.0f || correctionAmount <= 0.0f)
        return;

    // Calculate pitch shift in semitones (-12 to +12)
    float pitchShiftSemitones = 12.0f * std::log2(shiftRatio);
    pitchShiftSemitones = std::clamp(pitchShiftSemitones, -12.0f, 12.0f);
    
    // Apply correction amount (0-1 blend between original and shifted)
    pitchShiftSemitones *= correctionAmount;

    soundTouch->setPitchSemiTones(pitchShiftSemitones);
    soundTouch->putSamples((soundtouch::SAMPLETYPE*)audioData, numSamples);

    int outSamples = soundTouch->receiveSamples((soundtouch::SAMPLETYPE*)audioData, numSamples);
    
    // Zero out remaining samples if fewer were returned
    if (outSamples < numSamples)
    {
        std::fill(audioData + outSamples, audioData + numSamples, 0.0f);
    }
}