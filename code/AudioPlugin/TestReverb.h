#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "components/DelayLine.h"


class TestReverb
{
public:
    TestReverb();
    ~TestReverb();

    void prepareReverb(double sampleRate);

    void processReverb(juce::AudioBuffer<float>& buffer, int totalNumInputChannels);
    void updatePosition(juce::AudioBuffer<float>& buffer);
    juce::AudioBuffer<float>& getChannel();

private:
    // Reverb consists of (1) diffusion step (object)

    // (2) Late Feedback Loop Step (with delay lines)
    DelayLine channelA;
    DelayLine channelB;
    DelayLine channelC;
    DelayLine channelD;
};