#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "components/DelayLine.h"
#include "components/Diffuser.h"

class TestReverb
{
public:
    TestReverb();
    ~TestReverb();

    void prepareReverb(double sampleRate);
    void processReverb(juce::AudioBuffer<float>& buffer, int channel);
    void updatePosition(juce::AudioBuffer<float>& buffer);

private:
    // Reverb consists of a diffuser and feedback network (DelayLines)
    // Diffuser diffuserA;

    // (2) Late Feedback Loop Step (with delay lines)
    DelayLine channelA;
    DelayLine channelB;
    DelayLine channelC;
    DelayLine channelD;
};