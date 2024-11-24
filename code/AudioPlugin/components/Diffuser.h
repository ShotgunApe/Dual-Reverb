#pragma once

#include "DelayLine.h"

// Diffusion takes an input signal copied over several channels in parallel, delays the signal, shuffles, inverts, and redistributes it

class Diffuser {
public:
    // Number of channels to use
    static const int mixChannels = 8;

    Diffuser(double sampleRate);
    ~Diffuser();

    void diffusionProcess(std::array<float, mixChannels>& slice); // Include more here eventually

private:
    // Matricies n stuff
    std::array<float, mixChannels * mixChannels> mixMatrix;
	std::array<float, mixChannels> flipPolarity;
	std::vector<std::unique_ptr<DelayLine>> mDelays;
};