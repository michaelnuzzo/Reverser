/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ReverserAudioProcessorEditor::ReverserAudioProcessorEditor (ReverserAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{

    addAndMakeVisible(timeKnob);
    timeKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    timeKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    timeKnob.onValueChange = [this] {audioProcessor.setUpdate();};
    timeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(),"TIME",timeKnob);
    
    

    addAndMakeVisible(dryWetKnob);
    dryWetKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    dryWetKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    dryWetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(),"DRYWET",dryWetKnob);

    setSize (400, 300);
}

ReverserAudioProcessorEditor::~ReverserAudioProcessorEditor()
{
}

//==============================================================================
void ReverserAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
}

void ReverserAudioProcessorEditor::resized()
{
    timeKnob.setBounds(100, 200, 100, 100);
    dryWetKnob.setBounds(300, 200, 100, 100);
}
