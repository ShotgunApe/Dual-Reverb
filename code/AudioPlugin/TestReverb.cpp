#include "TestReverb.h"

TestReverb::TestReverb()
{
}

TestReverb::~TestReverb()
{
}

void TestReverb::prepareReverb(double sampleRate)
{
    int tmp = 5000;
    // preparing delay line stuff
    auto delayBufferSize = sampleRate * 2;
    channelA.getBuffer().clear();
    channelA.getBuffer().setSize (2, (int) delayBufferSize); // output is STEREO (this caused me so much pain)

    // SET DELAY CANNOT BE SO LOW OR ELSE UNDEFINED BEHAVIOR HAPPENS
    // additoinal channels must be set in ratio of 1:1.5 (tmp * 1.5)
    channelA.setDelay(tmp);
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