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
    getLookAndFeel().setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    getLookAndFeel().setColour(juce::Label::textColourId, juce::Colours::black);
    getLookAndFeel().setColour(juce::ToggleButton::ColourIds::tickColourId, juce::Colours::black);
    getLookAndFeel().setColour(juce::ToggleButton::ColourIds::tickDisabledColourId, juce::Colours::black);

    timeKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    timeKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    timeKnob.onValueChange = [this] {audioProcessor.setUpdate();};
    timeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(),"TIME",timeKnob);
    timeLabel.setText("Window Length (ms)", juce::dontSendNotification);
    timeLabel.setJustificationType(juce::Justification::horizontallyCentred);
    timeLabel.attachToComponent(&timeKnob, false);
    addAndMakeVisible(timeKnob);

    dryWetKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    dryWetKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    dryWetAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getParameters(),"DRYWET",dryWetKnob);
    dryWetLabel.setText("Dry/Wet (%)", juce::dontSendNotification);
    dryWetLabel.setJustificationType(juce::Justification::horizontallyCentred);
    dryWetLabel.attachToComponent(&dryWetKnob, false);
    addAndMakeVisible(dryWetKnob);

    crossfadeButton.onClick = [this] {audioProcessor.setUpdate();};
    crossfadeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.getParameters(),"CROSSFADE",crossfadeButton);
    crossfadeLabel.setText("Crossfade", juce::dontSendNotification);
    crossfadeLabel.setJustificationType(juce::Justification::horizontallyCentred);
    crossfadeLabel.attachToComponent(&crossfadeButton, true);

    addAndMakeVisible(crossfadeButton);

    setResizable(true, true);
    setResizeLimits(400, 600, 1600, 900);
    setSize (800, 600);
}

ReverserAudioProcessorEditor::~ReverserAudioProcessorEditor()
{
}

//==============================================================================
void ReverserAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::aliceblue);
    g.setFont (15.0f);



    juce::Image logo = juce::ImageCache::getFromMemory (BinaryData::logo_png, BinaryData::logo_pngSize);
    logo = logo.rescaled(logo.getWidth()/2., logo.getHeight()/2.);
    float xPos = getWidth()/2.-logo.getWidth()/2.-8;
    float yPos = getHeight()/3.-logo.getHeight()/2.;

    g.drawImageAt(logo, xPos, yPos);
}

void ReverserAudioProcessorEditor::resized()
{
    float knobWidth = 150;
    float xPos1 = getWidth()/3.-knobWidth/2.;
    float xPos2 = 2*getWidth()/3.-knobWidth/2.;
    float yPos = getHeight()-300;

    timeKnob.setBounds(xPos1, yPos,knobWidth,knobWidth);
    dryWetKnob.setBounds(xPos2, yPos,knobWidth,knobWidth);
    crossfadeButton.setBounds(getWidth()/2, getHeight()-100, 100, 100);
}
