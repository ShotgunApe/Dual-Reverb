#include "TestReverb.h"
#include "juce_audio_basics/juce_audio_basics.h"

TestReverb::TestReverb()
{
}

TestReverb::~TestReverb()
{
}

void TestReverb::prepareReverb(double sampleRate)
{
    int tmp = 4027;
    // preparing delay line stuff
    auto delayBufferSize = sampleRate * 2;
    channelA.getBuffer().clear();
    channelA.getBuffer().setSize (2, (int) delayBufferSize); // output is STEREO (this caused me so much pain)

    channelB.getBuffer().clear();
    channelB.getBuffer().setSize (2, (int) delayBufferSize);

    channelC.getBuffer().clear();
    channelC.getBuffer().setSize (2, (int) delayBufferSize);

    channelD.getBuffer().clear();
    channelD.getBuffer().setSize (2, (int) delayBufferSize);

    combOne.clear();
    combOne.setSize(2, delayBufferSize);

    combTwo.clear();
    combTwo.setSize(2, delayBufferSize);

    combThr.clear();
    combThr.setSize(2, delayBufferSize);

    combFou.clear();
    combFou.setSize(2, delayBufferSize);

    // SET DELAY CANNOT BE SO LOW OR ELSE UNDEFINED BEHAVIOR HAPPENS
    // channels are set to prime numbers
    channelA.setDelay(tmp);
    channelB.setDelay((int) tmp - 100);
    channelC.setDelay((int) tmp + 1100);
    channelD.setDelay((int) tmp + 300);
}

void TestReverb::processReverb(juce::AudioBuffer<float>& buffer, int channel)
{
    // Start by holding the current dry signal as a temp var
    combOne.makeCopyOf(buffer, 1);
    combTwo.makeCopyOf(buffer, 1);
    combThr.makeCopyOf(buffer, 1);
    combFou.makeCopyOf(buffer, 1);

    // Complete a feedback loop 
    channelA.fillBuffer (combOne, channel);
    channelA.readFromBuffer (combOne, channelA.getBuffer(), channel);
    channelA.fillBuffer (combOne, channel);

    buffer.addFrom(channel, 0, combOne, channel, 0, combOne.getNumSamples());

    // Complete next feedback loop
    channelB.fillBuffer (combTwo, channel);
    channelB.readFromBuffer (combTwo, channelB.getBuffer(), channel);
    channelB.fillBuffer (combTwo, channel);

    buffer.addFrom(channel, 0, combTwo, channel, 0, combTwo.getNumSamples());

    channelC.fillBuffer (combThr, channel);
    channelC.readFromBuffer (combThr, channelC.getBuffer(), channel);
    channelC.fillBuffer (combThr, channel);

    buffer.addFrom(channel, 0, combThr, channel, 0, combThr.getNumSamples());

    // Complete next feedback loop
    channelD.fillBuffer (combFou, channel);
    channelD.readFromBuffer (combFou, channelD.getBuffer(), channel);
    channelD.fillBuffer (combFou, channel);

    buffer.addFrom(channel, 0, combFou, channel, 0, combFou.getNumSamples());

}

void TestReverb::updatePosition(juce::AudioBuffer<float>& buffer)
{
    channelA.updateBufferPosition (buffer, channelA.getBuffer());
    channelB.updateBufferPosition (buffer, channelB.getBuffer());
    channelC.updateBufferPosition (buffer, channelC.getBuffer());
    channelD.updateBufferPosition (buffer, channelD.getBuffer());
}