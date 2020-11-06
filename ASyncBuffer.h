/*
 ==============================================================================

 ASyncBuffer.h
 Created: 31 Oct 2020 12:00:21pm
 Author:  Michael Nuzzo

 ==============================================================================
 */

#pragma once

#include <JuceHeader.h>

class ASyncBuffer
{
public:
    ASyncBuffer();
    ~ASyncBuffer();

    void write(const juce::AudioBuffer<float>& inBuffer, int numToWrite = -1, int numToMark = -1, int ID = -1);
    int read(juce::AudioBuffer<float>& outBuffer, int numToRead = -1, int numToMark = -1, int ID = -1);
    void reset();
    inline int getNumUnread() {return abstractFifo.getNumReady();}

private:
    const int MAX_LENGTH = 300000;
    juce::AbstractFifo abstractFifo{MAX_LENGTH};
    juce::AudioBuffer<float> circularBuffer;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ASyncBuffer)
};

