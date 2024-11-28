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

    void prepareDiffuser(double sampleRate);
    void processDiffuser(juce::AudioBuffer<float>& buffer, int channel);
    void updatePosition(juce::AudioBuffer<float>& buffer);
    void invert(juce::AudioBuffer<float>& buffer, int channel);
    void shuffle();

private:
    // Matricies n stuff
    std::array<float, mixChannels * mixChannels> m_mixMatrix;
    std::array<float, mixChannels> m_flipPolarity;

    // Channels for Diffuser
    DelayLine diffuseA;
    DelayLine diffuseB;
    DelayLine diffuseC;
    DelayLine diffuseD;
};