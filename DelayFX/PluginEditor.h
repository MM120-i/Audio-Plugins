/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class A2AudioProcessorEditor : public juce::AudioProcessorEditor, private juce::Slider::Listener, private juce::ComboBox::Listener, private juce::Button::Listener
{
public:
    A2AudioProcessorEditor(A2AudioProcessor &);
    ~A2AudioProcessorEditor() override;

    void paint(juce::Graphics &) override;
    void resized() override;

private:
    void sliderValueChanged(juce::Slider *slider) override;
    void comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged) override;
    void buttonClicked(juce::Button *button) override;
    void setupTimeInterval();
    void setupDrySlider();
    void setupWetSlider();
    void setupFeedbackSlider();
    void setupZenoButton();
    void setupBottomLabel();

    A2AudioProcessor &audioProcessor;

    juce::Label timeIntervalLabel;
    juce::Label dryLabel;
    juce::Label wetLabel;
    juce::Label feedbackLabel;
    juce::Label zenoLabel;
    juce::ComboBox timeIntervalComboBox;
    juce::Slider drySlider;
    juce::Slider wetSlider;
    juce::Slider feedbackSlider;
    juce::ToggleButton zenoButton;
    juce::Label bottomLabel;
    juce::Label additionalLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(A2AudioProcessorEditor)
};
