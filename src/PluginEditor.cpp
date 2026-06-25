#include "PluginEditor.h"
#include "PluginProcessor.h"

FreeAutotuneEditor::FreeAutotuneEditor(FreeAutotuneProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(800, 500);

    // Create tab bar
    addAndMakeVisible(tabBar = std::make_unique<juce::TabbedButtonBar>(juce::TabbedButtonBar::Orientation::TabsAtTop));
    tabBar->addTab("Controls", juce::Colour::fromRGB(60, 60, 60), 0, false);
    tabBar->addTab("Piano Roll", juce::Colour::fromRGB(60, 60, 60), 1, false);
    tabBar->addListener(this);

    // Create control panel
    addAndMakeVisible(controlPanel = std::make_unique<juce::Component>());
    setupControls();
    setupLabels();

    // Create piano roll
    addAndMakeVisible(pianoRoll = std::make_unique<PianoRollComponent>(audioProcessor));
    pianoRoll->setVisible(false);
}

FreeAutotuneEditor::~FreeAutotuneEditor() {}

void FreeAutotuneEditor::setupControls()
{
    // Pitch Correction Slider
    controlPanel->addAndMakeVisible(pitchCorrectionSlider = std::make_unique<juce::Slider>());
    pitchCorrectionSlider->setRange(0.0, 1.0, 0.01);
    pitchCorrectionSlider->setValue(0.8);
    pitchCorrectionSlider->setSliderStyle(juce::Slider::LinearHorizontal);
    pitchCorrectionSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    pitchCorrectionSlider->onValueChange = [this]()
    {
        *audioProcessor.pitchCorrectionAmount = (float)pitchCorrectionSlider->getValue();
    };

    // Scale Type Selector
    controlPanel->addAndMakeVisible(scaleTypeSelector = std::make_unique<juce::ComboBox>());
    scaleTypeSelector->addItem("Major", 1);
    scaleTypeSelector->addItem("Minor", 2);
    scaleTypeSelector->addItem("Chromatic", 3);
    scaleTypeSelector->setSelectedId(1);
    scaleTypeSelector->onChange = [this]()
    {
        *audioProcessor.scaleType = (float)(scaleTypeSelector->getSelectedId() - 1);
    };

    // Bypass Button
    controlPanel->addAndMakeVisible(bypassButton = std::make_unique<juce::ToggleButton>());
    bypassButton->setButtonText("Bypass");
    bypassButton->onStateChange = [this]()
    {
        *audioProcessor.bypassAutotune = bypassButton->getToggleState();
    };

    // Vibrato Amount Slider
    controlPanel->addAndMakeVisible(vibratoAmountSlider = std::make_unique<juce::Slider>());
    vibratoAmountSlider->setRange(0.0, 1.0, 0.01);
    vibratoAmountSlider->setValue(0.0);
    vibratoAmountSlider->setSliderStyle(juce::Slider::LinearHorizontal);
    vibratoAmountSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    vibratoAmountSlider->onValueChange = [this]()
    {
        *audioProcessor.vibratoAmount = (float)vibratoAmountSlider->getValue();
    };

    // Vibrato Rate Slider
    controlPanel->addAndMakeVisible(vibratoRateSlider = std::make_unique<juce::Slider>());
    vibratoRateSlider->setRange(0.5, 10.0, 0.1);
    vibratoRateSlider->setValue(5.0);
    vibratoRateSlider->setSliderStyle(juce::Slider::LinearHorizontal);
    vibratoRateSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    vibratoRateSlider->onValueChange = [this]()
    {
        *audioProcessor.vibratoRate = (float)vibratoRateSlider->getValue();
    };

    // Formant Shift Slider
    controlPanel->addAndMakeVisible(formantShiftSlider = std::make_unique<juce::Slider>());
    formantShiftSlider->setRange(-12.0, 12.0, 0.1);
    formantShiftSlider->setValue(0.0);
    formantShiftSlider->setSliderStyle(juce::Slider::LinearHorizontal);
    formantShiftSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 50, 20);
    formantShiftSlider->onValueChange = [this]()
    {
        *audioProcessor.formantShift = (float)formantShiftSlider->getValue();
    };
}

void FreeAutotuneEditor::setupLabels()
{
    controlPanel->addAndMakeVisible(pitchLabel = std::make_unique<juce::Label>());
    pitchLabel->setText("Pitch Correction:", juce::dontSendNotification);
    pitchLabel->setJustificationType(juce::Justification::centredRight);

    controlPanel->addAndMakeVisible(scaleLabel = std::make_unique<juce::Label>());
    scaleLabel->setText("Scale:", juce::dontSendNotification);
    scaleLabel->setJustificationType(juce::Justification::centredRight);

    controlPanel->addAndMakeVisible(vibratoLabel = std::make_unique<juce::Label>());
    vibratoLabel->setText("Vibrato Amount:", juce::dontSendNotification);
    vibratoLabel->setJustificationType(juce::Justification::centredRight);

    controlPanel->addAndMakeVisible(vibratoRateLabel = std::make_unique<juce::Label>());
    vibratoRateLabel->setText("Vibrato Rate (Hz):", juce::dontSendNotification);
    vibratoRateLabel->setJustificationType(juce::Justification::centredRight);

    controlPanel->addAndMakeVisible(formantLabel = std::make_unique<juce::Label>());
    formantLabel->setText("Formant Shift (st):", juce::dontSendNotification);
    formantLabel->setJustificationType(juce::Justification::centredRight);
}

void FreeAutotuneEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour::fromRGB(40, 40, 40));
    g.setColour(juce::Colours::white);
    g.setFont(18.0f);
    g.drawText("FreeAutotune", 10, 5, 300, 30, juce::Justification::centredLeft);
}

void FreeAutotuneEditor::resized()
{
    auto area = getLocalBounds();
    
    // Tab bar at top
    tabBar->setBounds(area.removeFromTop(30));

    // Control panel layout
    if (controlPanel->isVisible())
    {
        controlPanel->setBounds(area);

        int labelWidth = 150;
        int sliderHeight = 40;
        int sliderSpacing = 15;

        auto controlArea = area.reduced(20);

        // Row 1: Pitch Correction
        pitchLabel->setBounds(controlArea.removeFromTop(sliderHeight).removeFromLeft(labelWidth));
        pitchCorrectionSlider->setBounds(controlArea.removeFromTop(sliderHeight).removeFromRight(300));
        controlArea.removeFromTop(sliderSpacing);

        // Row 2: Scale
        scaleLabel->setBounds(controlArea.removeFromTop(sliderHeight).removeFromLeft(labelWidth));
        scaleTypeSelector->setBounds(controlArea.removeFromTop(sliderHeight).removeFromRight(300));
        controlArea.removeFromTop(sliderSpacing);

        // Row 3: Vibrato Amount
        vibratoLabel->setBounds(controlArea.removeFromTop(sliderHeight).removeFromLeft(labelWidth));
        vibratoAmountSlider->setBounds(controlArea.removeFromTop(sliderHeight).removeFromRight(300));
        controlArea.removeFromTop(sliderSpacing);

        // Row 4: Vibrato Rate
        vibratoRateLabel->setBounds(controlArea.removeFromTop(sliderHeight).removeFromLeft(labelWidth));
        vibratoRateSlider->setBounds(controlArea.removeFromTop(sliderHeight).removeFromRight(300));
        controlArea.removeFromTop(sliderSpacing);

        // Row 5: Formant Shift
        formantLabel->setBounds(controlArea.removeFromTop(sliderHeight).removeFromLeft(labelWidth));
        formantShiftSlider->setBounds(controlArea.removeFromTop(sliderHeight).removeFromRight(300));
        controlArea.removeFromTop(sliderSpacing);

        // Row 6: Bypass
        bypassButton->setBounds(controlArea.removeFromTop(sliderHeight).removeFromRight(300));
    }

    // Piano roll layout
    if (pianoRoll->isVisible())
    {
        pianoRoll->setBounds(area);
    }
}

void FreeAutotuneEditor::currentTabChanged(int newCurrentTabIndex, const juce::String& tabName)
{
    if (newCurrentTabIndex == 0)
    {
        controlPanel->setVisible(true);
        pianoRoll->setVisible(false);
        audioProcessor.manualPitchMode.store(false);
    }
    else if (newCurrentTabIndex == 1)
    {
        controlPanel->setVisible(false);
        pianoRoll->setVisible(true);
        audioProcessor.manualPitchMode.store(true);
    }
}