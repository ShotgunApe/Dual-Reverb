#include "Diffuser.h"

#include "Matrix.h"
#include <chrono>
#include <random>

Diffuser::Diffuser ()
{
}

Diffuser::~Diffuser ()
{
}

void Diffuser::prepareDiffuser(double sampleRate)
{   
    auto delayBufferSize = sampleRate * 2;
    diffuseA.getBuffer().clear();
    diffuseA.getBuffer().setSize (2, (int) delayBufferSize); // output is STEREO (this caused me so much pain)
    diffuseB.getBuffer().clear();
    diffuseB.getBuffer().setSize (2, (int) delayBufferSize); // ditto
    diffuseC.getBuffer().clear();
    diffuseC.getBuffer().setSize (2, (int) delayBufferSize);
    diffuseD.getBuffer().clear();
    diffuseD.getBuffer().setSize (2, (int) delayBufferSize);

    // create random amount of delay length for delay line :)
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(10, 2880); // buffer duration ~0ms - 60ms (for 48000hz sample rate, need to fix any any sample rate)
    diffuseA.setDelay(dist6(rng));
    diffuseB.setDelay(dist6(rng));
    diffuseC.setDelay(dist6(rng));
    diffuseD.setDelay(dist6(rng));
}

void Diffuser::processDiffuser (juce::AudioBuffer<float>& buffer, int channel)
{   
    // delays with no feedback (*gasp*)
    diffuseA.fillBuffer (buffer, channel);
    diffuseA.readFromBuffer (buffer, diffuseA.getBuffer(), channel);

    diffuseB.fillBuffer (buffer, channel);
    diffuseB.readFromBuffer (buffer, diffuseB.getBuffer(), channel);

    diffuseC.fillBuffer (buffer, channel);
    diffuseC.readFromBuffer (buffer, diffuseC.getBuffer(), channel);

    diffuseD.fillBuffer (buffer, channel);
    diffuseD.readFromBuffer (buffer, diffuseD.getBuffer(), channel);

    // invert :3
    invert(buffer, channel);
}

void Diffuser::updatePosition(juce::AudioBuffer<float>& buffer)
{
    diffuseA.updateBufferPosition (buffer, diffuseA.getBuffer());
    diffuseB.updateBufferPosition (buffer, diffuseB.getBuffer());
    diffuseC.updateBufferPosition (buffer, diffuseC.getBuffer());
    diffuseD.updateBufferPosition (buffer, diffuseD.getBuffer());
}

void Diffuser::invert(juce::AudioBuffer<float>& buffer, int channel)
{
    for (int i = 0; i < diffuseA.getBuffer().getNumSamples(); ++i) {
        diffuseA.getBuffer().setSample(channel, i, (diffuseA.getBuffer().getSample(channel, i) * -1));
        diffuseC.getBuffer().setSample(channel, i, (diffuseC.getBuffer().getSample(channel, i) * -1));
    }
}

void Diffuser::shuffle()
{
}