#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

FreeAutotuneProcessor::FreeAutotuneProcessor()
{
    addParameter(pitchCorrectionAmount = new juce::AudioParameterFloat(
        "pitchCorrection", "Pitch Correction", 0.0f, 1.0f, 0.8f));
    
    addParameter(scaleType = new juce::AudioParameterChoice(
        "scaleType", "Scale", 
        juce::StringArray("Major", "Minor", "Chromatic"), 0));
    
    addParameter(bypassAutotune = new juce::AudioParameterBool(
        "bypass", "Bypass", false));

    addParameter(vibratoAmount = new juce::AudioParameterFloat(
        "vibratoAmount", "Vibrato Amount", 0.0f, 1.0f, 0.0f));

    addParameter(vibratoRate = new juce::AudioParameterFloat(
        "vibratoRate", "Vibrato Rate", 0.5f, 10.0f, 5.0f));

    addParameter(formantShift = new juce::AudioParameterFloat(
        "formantShift", "Formant Shift", -12.0f, 12.0f, 0.0f));

    pitchDetector = std::make_unique<PitchDetector>();
    pitchShifter = std::make_unique<PitchShifter>();
}

FreeAutotuneProcessor::~FreeAutotuneProcessor() {}

void FreeAutotuneProcessor::prepareToPlay(double newSampleRate, int newSamplesPerBlock)
{
    sampleRate = newSampleRate;
    samplesPerBlock = newSamplesPerBlock;

    pitchDetector->prepare(sampleRate, samplesPerBlock);
    pitchShifter->prepare(sampleRate);
    vibratoPhase = 0.0f;
}

void FreeAutotuneProcessor::releaseResources() {}

float FreeAutotuneProcessor::applyVibrato(float frequency, float rate, float amount)
{
    // LFO-based vibrato (sine wave modulation)
    float lfo = std::sin(2.0f * juce::MathConstants<float>::pi * vibratoPhase);
    float maxDeviation = frequency * 0.05f * amount; // Max 5% of frequency
    return frequency + (lfo * maxDeviation);
}

void FreeAutotuneProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    if (bypassAutotune->get())
        return;

    // Process first channel only for now
    if (buffer.getNumChannels() > 0)
    {
        auto* channelData = buffer.getWritePointer(0);

        // Detect pitch
        float detectedFreq = pitchDetector->detectPitch(channelData, buffer.getNumSamples());
        detectedPitch.store(detectedFreq);

        // Use manual pitch if in manual mode, otherwise auto-quantize
        float targetFreq;
        if (manualPitchMode.load())
        {
            targetFreq = manualTargetPitch.load();
        }
        else
        {
            targetFreq = pitchDetector->quantizeToScale(
                detectedFreq, 
                static_cast<PitchDetector::Scale>(scaleType->getIndex()));
        }

        // Apply vibrato
        float vibratoAmount_val = vibratoAmount->get();
        float vibratoRate_val = vibratoRate->get();
        if (vibratoAmount_val > 0.0f)
        {
            targetFreq = applyVibrato(targetFreq, vibratoRate_val, vibratoAmount_val);
            vibratoPhase += vibratoRate_val / (sampleRate / samplesPerBlock);
            if (vibratoPhase >= 1.0f)
                vibratoPhase -= 1.0f;
        }

        targetPitch.store(targetFreq);

        // Shift pitch
        float correctionAmount = pitchCorrectionAmount->get();
        if (detectedFreq > 0.0f)
        {
            float shiftRatio = targetFreq / detectedFreq;
            
            // Apply formant shift
            float formantAmount = formantShift->get();
            if (formantAmount != 0.0f)
            {
                // Preserve formants by applying additional ratio adjustment
                shiftRatio *= std::pow(2.0f, formantAmount / 12.0f);
            }

            pitchShifter->processPitchShift(
                channelData, 
                buffer.getNumSamples(),
                shiftRatio,
                correctionAmount);
        }
    }
}

juce::AudioProcessorEditor* FreeAutotuneProcessor::createEditor()
{
    return new FreeAutotuneEditor(*this);
}

void FreeAutotuneProcessor::getStateInformation(juce::MemoryBlock& destData) {}

void FreeAutotuneProcessor::setStateInformation(const void* data, int sizeInBytes) {}