#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "DelayLine.h"

// Diffusion takes an input signal copied over several channels in parallel, delays the signal, shuffles, inverts, and redistributes it

class Diffuser {
public:
    // Number of channels to use
    static const int mixChannels = 8;

    Diffuser();
    ~Diffuser();

    void processDiffuser(juce::AudioBuffer<float>& buffer, int sample);

private:
    // Matricies n stuff
    std::array<float, mixChannels * mixChannels> m_mixMatrix;
    std::array<float, mixChannels> m_flipPolarity;
    std::vector<DelayLine> m_delays;
};