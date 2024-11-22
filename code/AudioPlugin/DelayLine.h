#pragma once
#include "PluginProcessor.h"

class DelayLine
{
public:
    DelayLine();
    ~DelayLine();

    void fillBuffer (juce::AudioBuffer<float>& buffer, int channel);
    void readFromBuffer (juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& delayBuffer, int channel);
    void updateBufferPosition (juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& delayBuffer);

private:
    // For circular buffer
    juce::AudioBuffer<float> delayBuffer;
    int writePosition { 0 };
};