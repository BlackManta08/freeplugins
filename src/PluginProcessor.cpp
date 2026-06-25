#include "PluginProcessor.h"
#include "PluginEditor.h"

FreeAutotuneProcessor::FreeAutotuneProcessor()
{
    addParameter(pitchCorrectionAmount = new juce::AudioParameterFloat(
        "pitchCorrection", "Pitch Correction", 0.0f, 1.0f, 0.8f));
    
    addParameter(scaleType = new juce::AudioParameterChoice(
        "scaleType", "Scale", 
        juce::StringArray("Major", "Minor", "Chromatic"), 0));
    
    addParameter(bypassAutotune = new juce::AudioParameterBool(
        "bypass", "Bypass", false));

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
}

void FreeAutotuneProcessor::releaseResources() {}

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

        // Quantize to target pitch based on scale
        float targetFreq = pitchDetector->quantizeToScale(
            detectedFreq, 
            static_cast<PitchDetector::Scale>(scaleType->getIndex()));

        // Shift pitch
        float correctionAmount = pitchCorrectionAmount->get();
        float shiftRatio = targetFreq / detectedFreq;
        
        pitchShifter->processPitchShift(
            channelData, 
            buffer.getNumSamples(),
            shiftRatio,
            correctionAmount);
    }
}

juce::AudioProcessorEditor* FreeAutotuneProcessor::createEditor()
{
    return new FreeAutotuneEditor(*this);
}

void FreeAutotuneProcessor::getStateInformation(juce::MemoryBlock& destData) {}

void FreeAutotuneProcessor::setStateInformation(const void* data, int sizeInBytes) {}