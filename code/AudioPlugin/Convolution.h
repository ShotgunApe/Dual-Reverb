#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "juce_audio_basics/juce_audio_basics.h"

class Convolution
{
public:
    Convolution();
    ~Convolution();

    void prepareReverb(double sampleRate);
    void processReverb(juce::AudioBuffer<float>& buffer, int channel);
    void updatePosition(juce::AudioBuffer<float>& buffer);

private:
    

};