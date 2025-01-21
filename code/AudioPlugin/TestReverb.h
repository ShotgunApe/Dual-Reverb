#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "components/DelayLine.h"

class TestReverb
{
public:
    TestReverb();
    ~TestReverb();

    void prepareReverb(double sampleRate);
    void processReverb(juce::AudioBuffer<float>& buffer, int channel);
    void updatePosition(juce::AudioBuffer<float>& buffer);

private:
    // Late Feedback Loop Step (with delay lines)
    DelayLine channelA;
    
};