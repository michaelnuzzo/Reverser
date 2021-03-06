/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class ReverserAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ReverserAudioProcessorEditor (ReverserAudioProcessor&);
    ~ReverserAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ReverserAudioProcessor& audioProcessor;
    juce::Label timeLabel, dryWetLabel, crossfadeLabel, dryWetAlignLabel;
    juce::Slider timeKnob, dryWetKnob;
    juce::ToggleButton crossfadeButton, dryWetAlignButton;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> timeAttachment, dryWetAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> crossfadeAttachment, dryWetAlignAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverserAudioProcessorEditor)
};
