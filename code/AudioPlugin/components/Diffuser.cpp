#include "Diffuser.h"

#include "Matrix.h"
#include <chrono>
#include <random>

static constexpr float DELAY_TIME_MAX = 0.02f;

Diffuser::Diffuser (double sampleRate)
{
    // create delays with a randomized delay time
    auto seed = static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count());
    std::mt19937 rng(seed);
    for (int channel = 0; channel < mixChannels; ++channel) {
        const float delayTimeMin = DELAY_TIME_MAX / mixChannels * channel;
        const float delayTimeMax = DELAY_TIME_MAX / mixChannels * (channel + 1);
        float delayTime = std::uniform_real_distribution<float>(delayTimeMin, delayTimeMax)(rng);
        int numSamples = std::max(1, static_cast<int>(delayTime * sampleRate));
        mDelays.push_back(std::make_unique<DelayLine>(numSamples));
    }

    // create flip polarity flags
    std::uniform_int_distribution<int> binary_distribution(0, 1);
    for (int channel = 0; channel < mixChannels; ++channel) {
        //flipPolarity[channel] = binary_distribution(rng) * 2.f - 1.f;
    }

    mixMatrix = MatrixUtils::createRandomHouseholderMatrix<float, mixChannels>();
}

Diffuser::~Diffuser ()
{
}

void Diffuser::diffusionProcess (std::array<float, mixChannels>& slice)
{
    float tmp[mixChannels];
    for (int channel = 0; channel < mixChannels; ++channel) {
        //tmp[channel] = mDelays[channel]->getSample(channel) * flipPolarity[channel];
        //mDelays[channel]->write(slice[channel]);
        //mDelays[channel]->next();
    }

    float mixed[mixChannels];
    for (int row = 0; row < mixChannels; ++row) {
        mixed[row] = 0;
        for (int column = 0; column < mixChannels; ++column) {
            mixed[row] += tmp[column] * mixMatrix[row * mixChannels + column];
        }
    }

    for (int channel = 0; channel < mixChannels; ++channel) {
        slice[channel] = mixed[channel];
    }
}