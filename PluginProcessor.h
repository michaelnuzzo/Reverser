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
    juce::AudioProcessorValueTreeState parameters;
    juce::dsp::DryWetMixer<float> mixer;
    ASyncBuffer inWindow;
    ASyncBuffer outWindow;
    juce::AudioBuffer<float> subsection;
    juce::AudioBuffer<float> dspProcessor;
    bool requiresUpdate = true;
    float reverserLength; // in seconds
    int windowLength; // in samples
    const int NUM_CHANNELS = 2;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverserAudioProcessor)
};
