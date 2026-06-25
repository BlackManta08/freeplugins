#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class FreeAutotuneProcessor;

class FreeAutotuneEditor : public juce::AudioProcessorEditor
{
public:
    explicit FreeAutotuneEditor(FreeAutotuneProcessor&);
    ~FreeAutotuneEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    FreeAutotuneProcessor& audioProcessor;

    std::unique_ptr<juce::Slider> pitchCorrectionSlider;
    std::unique_ptr<juce::ComboBox> scaleTypeSelector;
    std::unique_ptr<juce::ToggleButton> bypassButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FreeAutotuneEditor)
};