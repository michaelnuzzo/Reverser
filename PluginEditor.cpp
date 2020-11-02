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
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    addAndMakeVisible(timeKnob);
    timeKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    timeKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
//    timeKnob.setRange(0.0f, 1000.0f, 0.1f);
//    timeKnob.setValue(500.f);
//    timeKnob.onValueChange = [this] {audioProcessor.setReverserLength((float)timeKnob.getValue());};
    timeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.getAVPTS(),"TIME",timeKnob);
    
    

    addAndMakeVisible(dryWetKnob);
    dryWetKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    dryWetKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
//    dryWetKnob.setRange(0.0f, 1.0f, 0.01f);
//    dryWetKnob.setValue(1.0f);
//    dryWetKnob.onValueChange = [this] {audioProcessor.setDryWet((float)dryWetKnob.getValue());};
    dryWetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.getAVPTS(),"DRYWET",dryWetKnob);


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
