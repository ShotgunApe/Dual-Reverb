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
    channelA.getBuffer().clear();
    channelB.getBuffer().clear();
    channelC.getBuffer().clear();
    channelD.getBuffer().clear();

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(sampleRate, sampleRate * 2); // define the range

    auto delayBufferSize = distr(gen);
    channelA.getBuffer().setSize (1, (int) delayBufferSize); // output is mono for now i think

    delayBufferSize = distr(gen);
    channelB.getBuffer().setSize (1, (int) delayBufferSize); // output is mono for now i think

    delayBufferSize = distr(gen);
    channelC.getBuffer().setSize (1, (int) delayBufferSize); // output is mono for now i think

    delayBufferSize = distr(gen);
    channelD.getBuffer().setSize (1, (int) delayBufferSize); // output is mono for now i think
}

void TestReverb::processReverb(juce::AudioBuffer<float>& buffer, int totalNumInputChannels)
{
    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
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
}

void TestReverb::updatePosition(juce::AudioBuffer<float>& buffer)
{
    channelA.updateBufferPosition (buffer, channelA.getBuffer());
    channelB.updateBufferPosition (buffer, channelB.getBuffer());
    channelC.updateBufferPosition (buffer, channelC.getBuffer());
    channelD.updateBufferPosition (buffer, channelD.getBuffer());
}

juce::AudioBuffer<float>& TestReverb::getChannel()
{
    return (channelA.getBuffer());
}