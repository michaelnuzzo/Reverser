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
class NewProjectAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    NewProjectAudioProcessor();
    ~NewProjectAudioProcessor() override;

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

    void setReverserLength(float newLength)
    {
        reverserLength = newLength;

        inWindow.reset();
        outWindow.reset();

        juce::AudioBuffer<float> initializeWithZeros;
        initializeWithZeros.setSize(numChannels, reverserLength);
        initializeWithZeros.clear();

        inWindow.read(initializeWithZeros);
        outWindow.read(initializeWithZeros);

        if(newLength*getSampleRate() > 0)
        {
            windowLength = reverserLength/1000.f*getSampleRate();
        }
        else
        {
            windowLength = 2;
        }
        dspProcessor.setSize(numChannels, windowLength);
    }

    inline void setDryWet(float newDryWet) {dryWet = newDryWet;}

    void runDSP()
    {
        inWindow.read(dspProcessor);
        mixer.setWetMixProportion(dryWet);
        mixer.pushDrySamples(dspProcessor);
        dspProcessor.reverse(0, dspProcessor.getNumSamples());
        mixer.mixWetSamples(dspProcessor);
        outWindow.write(dspProcessor);
    }

private:
    ASyncBuffer inWindow;
    ASyncBuffer outWindow;
    juce::dsp::DryWetMixer<float> mixer;
    juce::AudioBuffer<float> dspProcessor;
    float reverserLength = 1; // in seconds
    float dryWet = 100;
    int windowLength = 1;
    int numChannels = 2;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessor)
};
