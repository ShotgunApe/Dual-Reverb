#include <random>
#include "TestReverb.h"

TestReverb::TestReverb()
{
}

TestReverb::~TestReverb()
{
}

void TestReverb::prepareReverb(double sampleRate)
{
    // prepare diffuser
    diffuserA.prepareDiffuser(sampleRate);

    // preparing delay line stuff
    auto delayBufferSize = sampleRate * 2;
    channelA.getBuffer().clear();
    channelA.getBuffer().setSize (2, (int) delayBufferSize); // output is STEREO (this caused me so much pain)
    channelB.getBuffer().clear();
    channelB.getBuffer().setSize (2, (int) delayBufferSize); // ditto
    channelC.getBuffer().clear();
    channelC.getBuffer().setSize (2, (int) delayBufferSize);
    channelD.getBuffer().clear();
    channelD.getBuffer().setSize (2, (int) delayBufferSize);

    // create random amount of delay length for delay line :)
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(4800, 9600); // buffer duration 100ms - 200ms (for 48000hz sample rate, need to fix any any sample rate)
    channelA.setDelay(dist6(rng));
    channelB.setDelay(dist6(rng));
    channelC.setDelay(dist6(rng));
    channelD.setDelay(dist6(rng));
}

void TestReverb::processReverb(juce::AudioBuffer<float>& buffer, int channel)
{
    // Diffuse! (can specify number of steps with for loop in future)
    diffuserA.processDiffuser(buffer, channel);

    //Feedback Delays!
    channelA.fillBuffer (buffer, channel);
    channelA.readFromBuffer (buffer, channelA.getBuffer(), channel);
    channelA.fillBuffer (buffer, channel);

    channelB.fillBuffer (buffer, channel);
    channelB.readFromBuffer (buffer, channelB.getBuffer(), channel);
    channelB.fillBuffer (buffer, channel);

    channelC.fillBuffer (buffer, channel);
    channelC.readFromBuffer (buffer, channelC.getBuffer(), channel);
    channelC.fillBuffer (buffer, channel);

    channelD.fillBuffer (buffer, channel);
    channelD.readFromBuffer (buffer, channelD.getBuffer(), channel);
    channelD.fillBuffer (buffer, channel);
}

void TestReverb::updatePosition(juce::AudioBuffer<float>& buffer)
{
    diffuserA.updatePosition(buffer);

    channelA.updateBufferPosition (buffer, channelA.getBuffer());
    channelB.updateBufferPosition (buffer, channelB.getBuffer());
    channelC.updateBufferPosition (buffer, channelC.getBuffer());
    channelD.updateBufferPosition (buffer, channelD.getBuffer());
}