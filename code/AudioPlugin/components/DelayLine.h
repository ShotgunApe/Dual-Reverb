#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

class DelayLine
{
public:
    DelayLine(int size);
    ~DelayLine();

    void fillBuffer (juce::AudioBuffer<float>& buffer, int channel);
    void readFromBuffer (juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& delayBuffer, int channel);
    void updateBufferPosition (juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& delayBuffer);
    
    juce::AudioBuffer<float>& getBuffer ();

private:
    // For circular buffer
    juce::AudioBuffer<float> delayBuffer; 
    int writePosition { 0 };
};