#include "Diffuser.h"

#include "Matrix.h"
#include <chrono>
#include <random>

Diffuser::Diffuser ()
{
    auto seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 rng(seed);
    for (int channel = 0; channel < mixChannels; ++channel) {
        const float delayTimeMin = 0.2 / mixChannels * channel;
        const float delayTimeMax = 0.2 / mixChannels * (channel + 1);
        float delayTime = std::uniform_real_distribution<float>(delayTimeMin, delayTimeMax)(rng);
        int numSamples = std::max(1, static_cast<int>(delayTime * 48000));
        //m_delays[channel] = DelayLine(numSamples); // this is breaking things
    }

    std::uniform_int_distribution<int> binary_distribution(0, 1);
    for (int channel = 0; channel < mixChannels; ++channel) {
        m_flipPolarity[channel] = binary_distribution(rng) * 2.f - 1.f;
    }

    m_mixMatrix = MatrixUtils::createRandomHouseholderMatrix<float, mixChannels>();
}

Diffuser::~Diffuser ()
{
}

void Diffuser::processDiffuser (juce::AudioBuffer<float>& buffer, int sample)
{
    int position = 0;
    for(DelayLine& line : m_delays) {
        buffer.setSample(position, sample, line.getBuffer().getSample(position, sample) * m_flipPolarity[position]);
        position++;
    }

    for (int row = 0; row < mixChannels; ++row) {
        int position = 0;
        for(DelayLine& line : m_delays) {
            buffer.addSample(position, sample, line.getBuffer().getSample(position, sample) * m_mixMatrix[row * mixChannels * position]);
            position++;
        }
    }
}