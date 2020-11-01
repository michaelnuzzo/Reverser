/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    addAndMakeVisible(timeDial);
    timeDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    timeDial.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    timeDial.setRange(0.0f, 1000.0f, 0.1f);
    timeDial.setValue(500.f);
    timeDial.onValueChange = [this] {audioProcessor.setReverserLength((float)timeDial.getValue());};

    addAndMakeVisible(dryWetKnob);
    dryWetKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    dryWetKnob.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
    dryWetKnob.setRange(0.0f, 1.0f, 0.01f);
    dryWetKnob.setValue(1.0f);
    dryWetKnob.onValueChange = [this] {audioProcessor.setDryWet((float)dryWetKnob.getValue());};

    setSize (400, 300);
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
}

//==============================================================================
void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
}

void NewProjectAudioProcessorEditor::resized()
{
    timeDial.setBounds(100, 200, 100, 100);
    dryWetKnob.setBounds(300, 200, 100, 100);
}
