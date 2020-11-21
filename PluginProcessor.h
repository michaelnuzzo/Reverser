/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ASyncBuffer.h"

//==============================================================================
/**
*/
class ReverserAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    ReverserAudioProcessor();
    ~ReverserAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void runDSP();
    inline void setUpdate() {requiresUpdate = true;}
    void updateLength();

    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    juce::AudioProcessorValueTreeState& getParameters() {return parameters;}

private:
    int numChannels = 2;
    bool requiresUpdate = true;
    float reverserLength; // in seconds
    int windowLength; // in samples
    ASyncBuffer inWindow;
    ASyncBuffer outWindow;
    juce::AudioProcessorValueTreeState parameters;
    juce::dsp::DryWetMixer<float> mixer;
    juce::dsp::WindowingFunction<float> window{1, juce::dsp::WindowingFunction<float>::triangular};
    juce::AudioBuffer<float> subsection;
    juce::AudioBuffer<float> dspProcessor;
    juce::AudioBuffer<float> dryHalf;
    juce::AudioBuffer<float> curHalf;
    juce::AudioBuffer<float> lastHalf;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverserAudioProcessor)
};
