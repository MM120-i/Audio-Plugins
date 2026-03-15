/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <cstdint>
#include <JuceHeader.h>


class A2AudioProcessor : public juce::AudioProcessor
{
public:
    float dryLevel = 1.0f;
    float wetLevel = 0.5f;
    float feedbackLevel = 0.5f;

    A2AudioProcessor();
    ~A2AudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;
    juce::AudioProcessorEditor *createEditor() override;
    bool hasEditor() const override;
    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String &newName) override;
    void getStateInformation(juce::MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;
    void setTimeInterval(float newTimeInterval);
    void setZenoEnabled(bool isEnabled);

private:
    float rate;
    juce::AudioBuffer<float> delayBuffer;
    int delayBufferLength = 0;
    float timeInterval = 0.0f;
    int delayBufferWritePos = 0;
    bool zenoEnabled = false;
    float zenoFeedbackLevel = 0.4f;
    std::uint64_t counter = 0;
    float previousTimeInterval = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(A2AudioProcessor)
};
