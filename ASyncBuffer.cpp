/*
  ==============================================================================

    ASyncBuffer.cpp
    Created: 31 Oct 2020 12:00:21pm
    Author:  Michael Nuzzo

  ==============================================================================
*/

#include "ASyncBuffer.h"

ASyncBuffer::ASyncBuffer()
{
    circularBuffer.setSize(2,1000000);
}

ASyncBuffer::~ASyncBuffer()
{
}

void ASyncBuffer::write(const juce::AudioBuffer<float>& inBuffer)
{
    int start1, size1, start2, size2;
    abstractFifo.prepareToWrite(inBuffer.getNumSamples(), start1, size1, start2, size2);
    int numChannels = juce::jmin(inBuffer.getNumChannels(), circularBuffer.getNumChannels());

    if(size1 > 0)
    {
        for(int ch = 0; ch < numChannels; ch++)
        {
            circularBuffer.copyFrom(ch, start1, inBuffer, ch, 0, size1);
        }
    }
    if(size2 > 0)
    {
        for(int ch = 0; ch < numChannels; ch++)
        {
            circularBuffer.copyFrom(ch, start2, inBuffer, ch, 0, size2);
        }
    }
    abstractFifo.finishedWrite(size1+size2);
}

int ASyncBuffer::read(juce::AudioBuffer<float>& outBuffer)
{
    int start1, size1, start2, size2;
    abstractFifo.prepareToRead(outBuffer.getNumSamples(), start1, size1, start2, size2);
    int numChannels = juce::jmin(outBuffer.getNumChannels(), circularBuffer.getNumChannels());

    if(size1 > 0)
    {
        for(int ch = 0; ch < numChannels; ch++)
        {
            outBuffer.copyFrom(ch, 0, circularBuffer, ch, start1, size1);
        }
    }
    if(size2 > 0)
    {
        for(int ch = 0; ch < numChannels; ch++)
        {
            outBuffer.copyFrom(ch, 0, circularBuffer, ch, start2, size2);
        }
    }
    abstractFifo.finishedRead(size1+size2);
    return(size1+size2);
}

void ASyncBuffer::reset()
{
    circularBuffer.clear();
    abstractFifo.reset();
}




