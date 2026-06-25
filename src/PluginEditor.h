#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "PianoRollComponent.h"

class FreeAutotuneProcessor;

class FreeAutotuneEditor : public juce::AudioProcessorEditor, public juce::TabbedButtonBar::Listener
{
public:
    explicit FreeAutotuneEditor(FreeAutotuneProcessor&);
    ~FreeAutotuneEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void currentTabChanged(int newCurrentTabIndex, const juce::String& tabName) override;

private:
    FreeAutotuneProcessor& audioProcessor;

    // Tabs
    std::unique_ptr<juce::TabbedButtonBar> tabBar;
    std::unique_ptr<juce::Component> controlPanel;
    std::unique_ptr<PianoRollComponent> pianoRoll;

    // Control sliders
    std::unique_ptr<juce::Slider> pitchCorrectionSlider;
    std::unique_ptr<juce::ComboBox> scaleTypeSelector;
    std::unique_ptr<juce::ToggleButton> bypassButton;
    std::unique_ptr<juce::Slider> vibratoAmountSlider;
    std::unique_ptr<juce::Slider> vibratoRateSlider;
    std::unique_ptr<juce::Slider> formantShiftSlider;

    // Labels
    std::unique_ptr<juce::Label> pitchLabel;
    std::unique_ptr<juce::Label> scaleLabel;
    std::unique_ptr<juce::Label> vibratoLabel;
    std::unique_ptr<juce::Label> vibratoRateLabel;
    std::unique_ptr<juce::Label> formantLabel;

    void setupControls();
    void setupLabels();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FreeAutotuneEditor)
};