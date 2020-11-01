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

    void write(const juce::AudioBuffer<float>& inBuffer);
    int read(juce::AudioBuffer<float>& outBuffer);
    void reset();
    inline int getNumUnread() {return abstractFifo.getNumReady();}

private:
    juce::AbstractFifo abstractFifo{1000000};
    juce::AudioBuffer<float> circularBuffer;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ASyncBuffer)
};

