#pragma once

#include <vector>

class PitchDetector
{
public:
    enum class Scale { Major, Minor, Chromatic };

    PitchDetector();
    ~PitchDetector();

    void prepare(double sampleRate, int samplesPerBlock);
    float detectPitch(const float* audioData, int numSamples);
    float quantizeToScale(float frequency, Scale scale);

private:
    double sampleRate = 44100.0;
    int samplesPerBlock = 512;

    // Yin algorithm parameters
    float detectPitchYin(const float* audioData, int numSamples);
    static constexpr float YIN_THRESHOLD = 0.1f;

    // Scale note frequencies (A0 = 27.5 Hz, tuned to A440)
    static constexpr float NOTE_FREQUENCIES[12] = {
        27.5f,  // A
        29.14f, // A#
        30.87f, // B
        32.70f, // C
        34.65f, // C#
        36.71f, // D
        38.89f, // D#
        41.20f, // E
        43.65f, // F
        46.25f, // F#
        49.00f, // G
        51.96f  // G#
    };

    static constexpr int MAJOR_SCALE[7] = {0, 2, 4, 5, 7, 9, 11};    // A, B, C#, D, E, F#, G#
    static constexpr int MINOR_SCALE[7] = {0, 2, 3, 5, 7, 8, 10};    // A, B, C, D, E, F, G
};