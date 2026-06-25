#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <array>

class FreeAutotuneProcessor;

class PianoRollComponent : public juce::Component, public juce::Timer
{
public:
    explicit PianoRollComponent(FreeAutotuneProcessor& processor);
    ~PianoRollComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    void timerCallback() override;

private:
    FreeAutotuneProcessor& audioProcessor;

    // Piano roll display
    static constexpr int NUM_NOTES = 48;  // 4 octaves
    static constexpr float BASE_FREQUENCY = 65.41f; // C2
    static constexpr int PIXELS_PER_NOTE = 20;

    // Visual feedback
    std::array<float, NUM_NOTES> noteHeights {};
    float detectedPitchVisual = 0.0f;
    float targetPitchVisual = 0.0f;

    // Manual control
    bool isDragging = false;
    int lastSelectedNote = -1;

    float frequencyToNotePosition(float frequency);
    float notePositionToFrequency(int noteIndex, float yOffset);
    void updateNoteDisplay();
    void drawPiano(juce::Graphics& g);
    void drawPitchCurve(juce::Graphics& g);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PianoRollComponent)
};