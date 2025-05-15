#include "TestReverb.h"
#include "juce_audio_basics/juce_audio_basics.h"
#include "juce_core/juce_core.h"

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
    channelA.getBuffer().setSize (2, (int) delayBufferSize);

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

    channelA.setDelay((int) 2423);
    channelB.setDelay((int) 3121);
    channelC.setDelay((int) 4451);
    channelD.setDelay((int) 5903);
}

void TestReverb::processReverb(juce::AudioBuffer<float>& buffer, int channel, int size)
{
    // COMB FILTERS
    // Start by holding the current dry signal as a temp var
    combOne.makeCopyOf(buffer, 1);
    combTwo.makeCopyOf(buffer, 1);
    combThr.makeCopyOf(buffer, 1);
    combFou.makeCopyOf(buffer, 1);

    // Complete each feedback loop and add them to their respective comb filter
    channelA.fillBuffer (combOne, channel);
    channelA.readFromBuffer (combOne, channelA.getBuffer(), channel, size);
    channelA.fillBuffer (combOne, channel);

    // Once fed back, add each comb filter to the wet signal
    buffer.addFrom(channel, 0, combOne, channel, 0, combOne.getNumSamples(), -1.0f);

    channelB.fillBuffer (combTwo, channel);
    channelB.readFromBuffer (combTwo, channelB.getBuffer(), channel, size);
    channelB.fillBuffer (combTwo, channel);

    buffer.addFrom(channel, 0, combTwo, channel, 0, combTwo.getNumSamples(), -1.0f);

    channelC.fillBuffer (combThr, channel);
    channelC.readFromBuffer (combThr, channelC.getBuffer(), channel, size);
    channelC.fillBuffer (combThr, channel);

    buffer.addFrom(channel, 0, combThr, channel, 0, combThr.getNumSamples(), -1.0f);

    channelD.fillBuffer (combFou, channel);
    channelD.readFromBuffer (combFou, channelD.getBuffer(), channel, size);
    channelD.fillBuffer (combFou, channel);

    buffer.addFrom(channel, 0, combFou, channel, 0, combFou.getNumSamples(), -1.0f);

}

void TestReverb::updatePosition(juce::AudioBuffer<float>& buffer)
{
    channelA.updateBufferPosition (buffer, channelA.getBuffer());
    channelB.updateBufferPosition (buffer, channelB.getBuffer());
    channelC.updateBufferPosition (buffer, channelC.getBuffer());
    channelD.updateBufferPosition (buffer, channelD.getBuffer());
}