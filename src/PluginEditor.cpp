#include "PluginEditor.h"
#include "PluginProcessor.h"

FreeAutotuneEditor::FreeAutotuneEditor(FreeAutotuneProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(400, 300);

    // Pitch Correction Slider
    addAndMakeVisible(pitchCorrectionSlider = std::make_unique<juce::Slider>());
    pitchCorrectionSlider->setRange(0.0, 1.0, 0.01);
    pitchCorrectionSlider->setValue(0.8);
    pitchCorrectionSlider->setSliderStyle(juce::Slider::LinearHorizontal);
    pitchCorrectionSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);

    // Scale Type Selector
    addAndMakeVisible(scaleTypeSelector = std::make_unique<juce::ComboBox>());
    scaleTypeSelector->addItem("Major", 1);
    scaleTypeSelector->addItem("Minor", 2);
    scaleTypeSelector->addItem("Chromatic", 3);
    scaleTypeSelector->setSelectedId(1);

    // Bypass Button
    addAndMakeVisible(bypassButton = std::make_unique<juce::ToggleButton>());
    bypassButton->setButtonText("Bypass");
}

FreeAutotuneEditor::~FreeAutotuneEditor() {}

void FreeAutotuneEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGB(40, 40, 40));
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawText("FreeAutotune", 10, 10, 380, 30, juce::Justification::centred);
}

void FreeAutotuneEditor::resized()
{
    pitchCorrectionSlider->setBounds(10, 50, 380, 40);
    scaleTypeSelector->setBounds(10, 100, 380, 30);
    bypassButton->setBounds(10, 140, 380, 30);
}