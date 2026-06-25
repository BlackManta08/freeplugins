#include "PianoRollComponent.h"
#include "PluginProcessor.h"
#include <cmath>

PianoRollComponent::PianoRollComponent(FreeAutotuneProcessor& p)
    : audioProcessor(p)
{
    startTimer(30); // 30ms refresh rate for smooth animation
}

PianoRollComponent::~PianoRollComponent()
{
    stopTimer();
}

void PianoRollComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGB(30, 30, 30));

    drawPiano(g);
    drawPitchCurve(g);
}

void PianoRollComponent::resized()
{
    // Update layout
}

void PianoRollComponent::timerCallback()
{
    updateNoteDisplay();
    repaint();
}

float PianoRollComponent::frequencyToNotePosition(float frequency)
{
    if (frequency <= 0.0f)
        return 0.0f;

    // Convert frequency to semitones from BASE_FREQUENCY
    float semitones = 12.0f * std::log2(frequency / BASE_FREQUENCY);
    return semitones;
}

float PianoRollComponent::notePositionToFrequency(int noteIndex, float yOffset)
{
    float semitones = noteIndex + yOffset;
    return BASE_FREQUENCY * std::pow(2.0f, semitones / 12.0f);
}

void PianoRollComponent::updateNoteDisplay()
{
    detectedPitchVisual = audioProcessor.detectedPitch.load();
    targetPitchVisual = audioProcessor.targetPitch.load();
}

void PianoRollComponent::drawPiano(juce::Graphics& g)
{
    int pianoWidth = 60;
    int height = getHeight();

    // Draw piano keyboard on the left
    g.setColour(juce::Colour::fromRGB(50, 50, 50));
    g.fillRect(0, 0, pianoWidth, height);

    // Draw note labels
    g.setColour(juce::Colours::white);
    g.setFont(10.0f);

    const char* noteNames[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

    for (int i = 0; i < NUM_NOTES; ++i)
    {
        int y = height - (i + 1) * PIXELS_PER_NOTE;
        if (y >= 0 && y < height)
        {
            int noteName = i % 12;
            g.drawText(noteNames[noteName], 5, y, 50, PIXELS_PER_NOTE, juce::Justification::centred);
        }
    }

    // Draw grid in the main area
    int gridX = pianoWidth;
    g.setColour(juce::Colour::fromRGB(60, 60, 60));

    for (int i = 0; i < NUM_NOTES; ++i)
    {
        int y = height - (i + 1) * PIXELS_PER_NOTE;
        if (y >= 0 && y < height)
        {
            if (i % 12 == 0)
                g.setColour(juce::Colour::fromRGB(80, 80, 80)); // Octave marker
            else
                g.setColour(juce::Colour::fromRGB(60, 60, 60));

            g.drawHorizontalLine(y, gridX, getWidth());
        }
    }
}

void PianoRollComponent::drawPitchCurve(juce::Graphics& g)
{
    int pianoWidth = 60;
    int height = getHeight();

    // Draw detected pitch
    if (detectedPitchVisual > 0.0f)
    {
        float notePos = frequencyToNotePosition(detectedPitchVisual);
        int pixelY = height - (notePos * PIXELS_PER_NOTE);

        g.setColour(juce::Colour::fromRGB(100, 150, 255)); // Blue for detected
        g.drawHorizontalLine(pixelY, pianoWidth, getWidth());
    }

    // Draw target pitch
    if (targetPitchVisual > 0.0f)
    {
        float notePos = frequencyToNotePosition(targetPitchVisual);
        int pixelY = height - (notePos * PIXELS_PER_NOTE);

        g.setColour(juce::Colour::fromRGB(255, 150, 100)); // Orange for target
        g.fillRect(pianoWidth, pixelY - 2, getWidth() - pianoWidth, 4);
    }

    // Draw center line (reference pitch)
    g.setColour(juce::Colour::fromRGB(80, 80, 80));
    g.drawHorizontalLine(height / 2, pianoWidth, getWidth());
}

void PianoRollComponent::mouseDown(const juce::MouseEvent& e)
{
    isDragging = true;
    mouseDrag(e);
}

void PianoRollComponent::mouseDrag(const juce::MouseEvent& e)
{
    if (!isDragging)
        return;

    int pianoWidth = 60;
    int height = getHeight();

    if (e.x > pianoWidth)
    {
        // Calculate which note is being clicked
        float noteFloat = (float)(height - e.y) / PIXELS_PER_NOTE;
        float frequency = notePositionToFrequency((int)noteFloat, noteFloat - (int)noteFloat);

        audioProcessor.manualPitchMode.store(true);
        audioProcessor.manualTargetPitch.store(frequency);
        lastSelectedNote = (int)noteFloat;
    }
}

void PianoRollComponent::mouseUp(const juce::MouseEvent& e)
{
    isDragging = false;
}