/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessor::NewProjectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
                    , avpts(*this, nullptr, "Parameters", createParameters())
{
    avpts.addParameterListener("TIME", this);
    avpts.addParameterListener("DRYWET", this);
    avpts.state = juce::ValueTree("Parameters");
}

NewProjectAudioProcessor::~NewProjectAudioProcessor()
{
}

//==============================================================================
const juce::String NewProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NewProjectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NewProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NewProjectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NewProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NewProjectAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NewProjectAudioProcessor::getProgramName (int index)
{
    return {};
}

void NewProjectAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void NewProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = 1000000;
    spec.sampleRate = sampleRate;
    spec.numChannels = numChannels;
    mixer.prepare(spec);
    mixer.reset();
}

void NewProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NewProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void NewProjectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    if(windowLength > 1)
    {
        for(int i = 0; i <= int(buffer.getNumSamples()/windowLength); i++)
        {
            juce::AudioBuffer<float> subsection;
            int startIdx = i*windowLength;
            int endIdx = juce::jmin((i+1)*windowLength-1, buffer.getNumSamples()-1);
            subsection.setSize(numChannels, (endIdx-startIdx)+1);
            for(int ch = 0; ch < numChannels; ch++)
            {
                subsection.copyFrom(ch, 0, buffer, ch, startIdx, subsection.getNumSamples());
            }
            inWindow.write(subsection);

            if(inWindow.getNumUnread() >= windowLength)
            {
                runDSP();
            }
        }
        if(outWindow.getNumUnread() >= buffer.getNumSamples())
        {
            outWindow.read(buffer);
        }
    }
}

//==============================================================================
bool NewProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NewProjectAudioProcessor::createEditor()
{
    return new NewProjectAudioProcessorEditor (*this);
}

//==============================================================================
void NewProjectAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    std::unique_ptr<juce::XmlElement> xml(avpts.state.createXml());
    copyXmlToBinary(*xml, destData);
}

void NewProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if(xmlState.get() != nullptr)
    {
        if(xmlState->hasTagName("Parameters"))
        {
            avpts.state = juce::ValueTree::fromXml(*xmlState);
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}

//void NewProjectAudioProcessor::setReverserLength(float newLength)
//{
//    reverserLength = newLength;
//
//    inWindow.reset();
//    outWindow.reset();
//
//    juce::AudioBuffer<float> initializeWithZeros;
//    initializeWithZeros.setSize(numChannels, reverserLength);
//    initializeWithZeros.clear();
//
//    inWindow.read(initializeWithZeros);
//    outWindow.read(initializeWithZeros);
//
//    if(newLength*getSampleRate() > 0)
//    {
//        windowLength = reverserLength/1000.f*getSampleRate();
//    }
//    else
//    {
//        windowLength = 2;
//    }
//    dspProcessor.setSize(numChannels, windowLength);
//}

void NewProjectAudioProcessor::runDSP()
{
    inWindow.read(dspProcessor);
    mixer.setWetMixProportion(*avpts.getRawParameterValue("DRYWET"));
    mixer.pushDrySamples(dspProcessor);
    dspProcessor.reverse(0, dspProcessor.getNumSamples());
    mixer.mixWetSamples(dspProcessor);
    outWindow.write(dspProcessor);
}

juce::AudioProcessorValueTreeState::ParameterLayout NewProjectAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    params.push_back(std::make_unique<juce::AudioParameterFloat>("TIME","Time", 0.f, 1000.f, 500.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DRYWET","DryWet", 0.f, 1.f, 1.f));
    return { params.begin(), params.end() };
}

void NewProjectAudioProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if(parameterID == "TIME")
    {
        reverserLength = newValue;

        inWindow.reset();
        outWindow.reset();

        juce::AudioBuffer<float> initializeWithZeros;
        initializeWithZeros.setSize(numChannels, reverserLength);
        initializeWithZeros.clear();

        inWindow.read(initializeWithZeros);
        outWindow.read(initializeWithZeros);

        if(reverserLength*getSampleRate() > 0)
        {
            windowLength = reverserLength/1000.f*getSampleRate();
        }
        else
        {
            windowLength = 2;
        }
        dspProcessor.setSize(numChannels, windowLength);
    }
    if(parameterID == "DRYWET")
    {
        dryWet = newValue;
    }
}
