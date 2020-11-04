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
    timeLabel.setText("Window Length (ms)", juce::dontSendNotification);
    timeLabel.setJustificationType(juce::Justification::horizontallyCentred);
    timeLabel.attachToComponent(&timeKnob, false);

    addAndMakeVisible(dryWetKnob);
    dryWetKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    dryWetKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    dryWetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(),"DRYWET",dryWetKnob);
    dryWetLabel.setText("Dry/Wet (%)", juce::dontSendNotification);
    dryWetLabel.setJustificationType(juce::Justification::horizontallyCentred);
    dryWetLabel.attachToComponent(&dryWetKnob, false);

    setResizable(true, true);
    setResizeLimits(400, 300, 1600, 900);
    setSize (800, 600);
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
    float knobWidth = 150;
    float xPos1 = getWidth()/3.-knobWidth/2.;
    float xPos2 = 2*getWidth()/3.-knobWidth/2.;
    float yPos = getHeight()-300;

    timeKnob.setBounds(xPos1, yPos,knobWidth,knobWidth);
    dryWetKnob.setBounds(xPos2, yPos,knobWidth,knobWidth);
}
