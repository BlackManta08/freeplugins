#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "PitchDetector.h"
#include "PitchShifter.h"
#include <atomic>

class FreeAutotuneProcessor : public juce::AudioProcessor
{
public:
    FreeAutotuneProcessor();
    ~FreeAutotuneProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "FreeAutotune"; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return "Default"; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // Parameters
    juce::AudioParameterFloat* pitchCorrectionAmount;
    juce::AudioParameterChoice* scaleType;
    juce::AudioParameterBool* bypassAutotune;
    juce::AudioParameterFloat* vibratoAmount;
    juce::AudioParameterFloat* vibratoRate;
    juce::AudioParameterFloat* formantShift;

    // Piano roll data
    std::atomic<float> detectedPitch { 0.0f };
    std::atomic<float> targetPitch { 0.0f };
    std::atomic<bool> manualPitchMode { false };
    std::atomic<float> manualTargetPitch { 0.0f };

private:
    std::unique_ptr<PitchDetector> pitchDetector;
    std::unique_ptr<PitchShifter> pitchShifter;

    double sampleRate = 44100.0;
    int samplesPerBlock = 512;

    // Vibrato
    float vibratoPhase = 0.0f;
    float applyVibrato(float frequency, float rate, float amount);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FreeAutotuneProcessor)
};