/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ReverserAudioProcessor::ReverserAudioProcessor()
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
                    , parameters(*this, nullptr, "Parameters", createParameters())
{
    parameters.state = juce::ValueTree("Parameters");
}

ReverserAudioProcessor::~ReverserAudioProcessor()
{
}

//==============================================================================
const juce::String ReverserAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ReverserAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ReverserAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ReverserAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ReverserAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ReverserAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ReverserAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ReverserAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ReverserAudioProcessor::getProgramName (int index)
{
    return {};
}

void ReverserAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ReverserAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    numChannels = getTotalNumInputChannels();
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = 1000000;
    spec.sampleRate = sampleRate;
    spec.numChannels = numChannels;
    mixer.prepare(spec);
    mixer.reset();
    setUpdate();
}

void ReverserAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ReverserAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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
void ReverserAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    if(requiresUpdate)
    {
        updateLength();
    }

    if(windowLength/2 > 1)
    {
        for(int i = 0; i <= int(buffer.getNumSamples()-1)/(windowLength/2); i++)
        {
            // if windowLength > bufferLength, then this only runs once and
            // startIdx = 0, endIdx = bufferLength, subsectionSize = bufferLength
            // if windowLength < bufferLength then this runs as many times
            // as needed to read the full buffer and
            // startIdx = i*windowLength, endIdx = start + i*windowLength, subsectionSize = windowLength
            // This loop is designed to always read the full bufferLength, while also processing every windowLength.

            int startIdx = i*(windowLength/2);
            int endIdx = juce::jmin((i+1)*(windowLength/2)-1, buffer.getNumSamples()-1);
            auto sub = juce::dsp::AudioBlock<float>(buffer).getSubBlock(startIdx, (endIdx+1)-startIdx);
            inWindow.push(sub);

            if(inWindow.getNumUnread() >= windowLength)
            {
                runDSP();
            }
        }

        if(outWindow.getNumUnread() >= buffer.getNumSamples())
        {
            outWindow.pop(buffer);
        }
        else
        {
            buffer.clear();
        }
    }
}

//==============================================================================
bool ReverserAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ReverserAudioProcessor::createEditor()
{
    return new ReverserAudioProcessorEditor (*this);
}

//==============================================================================
void ReverserAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    std::unique_ptr<juce::XmlElement> xml(parameters.state.createXml());
    copyXmlToBinary(*xml, destData);
}

void ReverserAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if(xmlState.get() != nullptr)
    {
        if(xmlState->hasTagName(parameters.state.getType()))
        {
            parameters.state = juce::ValueTree::fromXml(*xmlState);
        }
    }
    setUpdate();
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverserAudioProcessor();
}

//****************************************************************************//

juce::AudioProcessorValueTreeState::ParameterLayout ReverserAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    params.push_back(std::make_unique<juce::AudioParameterFloat>("TIME","Time", juce::NormalisableRange<float>(0.f,1000.f,0.1f), 500.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DRYWET","DryWet", juce::NormalisableRange<float>(0.f,1.f,0.001f), 1.f));
    return { params.begin(), params.end() };
}

void ReverserAudioProcessor::updateLength()
{
    reverserLength = *parameters.getRawParameterValue("TIME");

    if(reverserLength*getSampleRate() > 0)
    {
        windowLength = reverserLength/1000.f*getSampleRate();
        windowLength = 2*(int)(windowLength/2.);
    }
    else
    {
        windowLength = 2;
    }

    dspProcessor.setSize(numChannels, windowLength);
    dspMemory.setSize(numChannels, windowLength/2);
    dspProcessor.clear();
    dspMemory.clear();

    inWindow.reset();
    outWindow.reset();

    juce::AudioBuffer<float> initializeWithZeros;
    initializeWithZeros.setSize(numChannels, windowLength/2);
    initializeWithZeros.clear();
    inWindow.push(initializeWithZeros);
    outWindow.push(initializeWithZeros);

    window.fillWindowingTables(windowLength, juce::dsp::WindowingFunction<float>::triangular);
    
    requiresUpdate = false;
}

void ReverserAudioProcessor::runDSP()
{
    inWindow.pop(dspProcessor,dspProcessor.getNumSamples(),dspProcessor.getNumSamples()/2);

    auto currentFrame = juce::dsp::AudioBlock<float>(dspProcessor);
    auto firstHalf = currentFrame.getSubBlock(0, windowLength/2);
    auto secondHalf = currentFrame.getSubBlock((windowLength/2)-1, windowLength/2);
    auto prevHalf = juce::dsp::AudioBlock<float>(dspMemory);
    
    mixer.setWetMixProportion(*parameters.getRawParameterValue("DRYWET"));
    mixer.pushDrySamples(firstHalf);
    dspProcessor.reverse(0, dspProcessor.getNumSamples());

    for(int ch = 0; ch < numChannels; ch++)
        window.multiplyWithWindowingTable(currentFrame.getChannelPointer(ch), windowLength);

    firstHalf.add(prevHalf);
    firstHalf.multiplyBy(0.5);
    mixer.mixWetSamples(firstHalf);
    outWindow.push(firstHalf);

    prevHalf.copyFrom(secondHalf);
}
