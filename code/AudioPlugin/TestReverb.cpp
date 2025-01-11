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
    // preparing delay line stuff
    auto delayBufferSize = sampleRate * 2;
    channelA.getBuffer().clear();
    channelA.getBuffer().setSize (2, (int) delayBufferSize); // output is STEREO (this caused me so much pain)

    // create random amount of delay length for delay line :)
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(4800, 9600); // buffer duration 100ms - 200ms (for 48000hz sample rate, need to fix any any sample rate)
    channelA.setDelay(dist6(rng));
}

void TestReverb::processReverb(juce::AudioBuffer<float>& buffer, int channel)
{
    //Feedback Delays!
    channelA.fillBuffer (buffer, channel);
    channelA.readFromBuffer (buffer, channelA.getBuffer(), channel);
    channelA.fillBuffer (buffer, channel);
}

void TestReverb::updatePosition(juce::AudioBuffer<float>& buffer)
{
    channelA.updateBufferPosition (buffer, channelA.getBuffer());
}