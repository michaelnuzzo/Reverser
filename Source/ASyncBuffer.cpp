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
    circularBuffer.setSize(2,MAX_LENGTH);
}

ASyncBuffer::~ASyncBuffer()
{
}

void ASyncBuffer::push(const juce::AudioBuffer<float>& inBuffer, int numToWrite, int numToMark, int ID)
{
    if(numToWrite < 0)
    {
        numToWrite = inBuffer.getNumSamples();
    }
    if(numToMark < 0)
    {
        numToMark = numToWrite;
    }

    int start1, size1, start2, size2;
    abstractFifo.prepareToWrite(numToWrite, start1, size1, start2, size2);
    jassert(numToWrite == size1+size2);
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
            circularBuffer.copyFrom(ch, start2, inBuffer, ch, size1, size2);
        }
    }

    abstractFifo.finishedWrite(numToMark);
}

int ASyncBuffer::pop(juce::AudioBuffer<float>& outBuffer, int numToRead, int numToMark, int ID)
{
    if(numToRead < 0)
    {
        numToRead = outBuffer.getNumSamples();
    }
    if(numToMark < 0)
    {
        numToMark = numToRead;
    }

    int start1, size1, start2, size2;
    abstractFifo.prepareToRead(numToRead, start1, size1, start2, size2);
    jassert(numToRead == size1+size2);
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
            outBuffer.copyFrom(ch, size1, circularBuffer, ch, start2, size2);
        }
    }

    abstractFifo.finishedRead(numToMark);
    return(size1+size2);
}

void ASyncBuffer::reset()
{
    circularBuffer.clear();
    abstractFifo.reset();
}




