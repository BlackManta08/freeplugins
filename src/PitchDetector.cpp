#include "PitchDetector.h"
#include <cmath>
#include <algorithm>

constexpr float PitchDetector::NOTE_FREQUENCIES[12];
constexpr int PitchDetector::MAJOR_SCALE[7];
constexpr int PitchDetector::MINOR_SCALE[7];

PitchDetector::PitchDetector() {}

PitchDetector::~PitchDetector() {}

void PitchDetector::prepare(double newSampleRate, int newSamplesPerBlock)
{
    sampleRate = newSampleRate;
    samplesPerBlock = newSamplesPerBlock;
}

float PitchDetector::detectPitch(const float* audioData, int numSamples)
{
    return detectPitchYin(audioData, numSamples);
}

float PitchDetector::detectPitchYin(const float* audioData, int numSamples)
{
    // Simplified Yin algorithm for pitch detection
    int bufferSize = std::min(numSamples, (int)sampleRate / 50); // ~20ms window min
    
    if (bufferSize < 100)
        return 0.0f;

    std::vector<float> autoCorr(bufferSize, 0.0f);

    // Calculate autocorrelation
    for (int lag = 0; lag < bufferSize; ++lag)
    {
        for (int i = 0; i < bufferSize - lag; ++i)
        {
            autoCorr[lag] += audioData[i] * audioData[i + lag];
        }
    }

    // Find the first lag with value below threshold
    float minValue = autoCorr[0];
    int minLag = 0;

    for (int lag = 1; lag < bufferSize / 2; ++lag)
    {
        if (autoCorr[lag] < autoCorr[0] * YIN_THRESHOLD)
        {
            minLag = lag;
            break;
        }
        if (autoCorr[lag] < minValue)
        {
            minValue = autoCorr[lag];
            minLag = lag;
        }
    }

    if (minLag == 0)
        return 0.0f;

    // Convert lag to frequency
    float frequency = (float)sampleRate / minLag;
    
    // Validate frequency range (human voice typically 80Hz - 400Hz)
    if (frequency < 50.0f || frequency > 2000.0f)
        return 0.0f;

    return frequency;
}

float PitchDetector::quantizeToScale(float frequency, Scale scale)
{
    if (frequency < 20.0f)
        return 0.0f;

    // Find which octave we're in
    float noteIndex = 12.0f * std::log2(frequency / 27.5f);
    int octave = (int)(noteIndex / 12.0f);
    float noteInOctave = std::fmod(noteIndex, 12.0f);

    // Find closest note
    int closestNote = (int)(noteInOctave + 0.5f);
    closestNote = std::max(0, std::min(11, closestNote));

    // Quantize to scale
    const int* targetScale = (scale == Scale::Major) ? MAJOR_SCALE : 
                             (scale == Scale::Minor) ? MINOR_SCALE :
                             nullptr;

    if (targetScale == nullptr)
    {
        // Chromatic - just round to nearest note
        return NOTE_FREQUENCIES[closestNote] * (1 << octave);
    }

    // Find closest note in scale
    int bestScaleNote = MAJOR_SCALE[0];
    float bestDistance = 12.0f;

    for (int i = 0; i < 7; ++i)
    {
        int scaleNote = targetScale[i];
        float distance = std::abs((float)scaleNote - noteInOctave);
        if (distance > 6.0f)
            distance = 12.0f - distance;

        if (distance < bestDistance)
        {
            bestDistance = distance;
            bestScaleNote = scaleNote;
        }
    }

    return NOTE_FREQUENCIES[bestScaleNote] * (1 << octave);
}