#include "DelayLine.h"

DelayLine::DelayLine ()
{
}

DelayLine::~DelayLine ()
{
}

void DelayLine::fillBuffer (juce::AudioBuffer<float>& buffer, int channel) {
    auto bufferSize = buffer.getNumSamples();
    auto delayBufferSize = delayBuffer.getNumSamples();

    if (delayBufferSize > bufferSize + writePosition)
    {
        delayBuffer.copyFrom (channel, writePosition, buffer.getWritePointer(channel), bufferSize);
    }
    else
    {
        auto numSamplesToEnd = delayBufferSize - writePosition;
        delayBuffer.copyFrom (channel, writePosition, buffer.getWritePointer(channel), numSamplesToEnd);
        
        auto numSamplesAtStart = bufferSize - numSamplesToEnd;
        delayBuffer.copyFrom (channel, 0, buffer.getWritePointer(channel, numSamplesToEnd), numSamplesAtStart);
    }
}

void DelayLine::readFromBuffer (juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& delayBuffer, int channel)
{
    auto bufferSize = buffer.getNumSamples();
    auto delayBufferSize = delayBuffer.getNumSamples();

    // cant have negative array index ... wrap back circular buffer to get previous buffer
    auto readPosition = writePosition - delayTime;
    if (readPosition < 0) {
        readPosition += delayBufferSize;
    }

    // Calculate g based on length of delay itself to prevent different rates of decay (in the wise words of Will Pirkle, would otherwise "fail miserably")
    // Also hard-coding sample rate which is BAD but will keep for now
    // RT_60 (157350) should be changed to variable which will (eventually) be parameter to change when setting "room size"
    auto g = pow(10, ((double)(-3 * ((double) delayTime) / 1273500)));
    if (readPosition + bufferSize < delayBufferSize) {
        buffer.addFromWithRamp (channel, 0, delayBuffer.getReadPointer (channel, readPosition), bufferSize, g, g);
    }
    else {
        auto numSamplesToEnd = delayBufferSize - readPosition;
        buffer.addFromWithRamp (channel, 0, delayBuffer.getReadPointer (channel, readPosition), numSamplesToEnd, g, g);
    
        auto numSamplesAtStart = bufferSize - numSamplesToEnd;
        buffer.addFromWithRamp (channel, numSamplesToEnd, delayBuffer.getReadPointer (channel, 0), numSamplesAtStart, g, g);
    }
}

void DelayLine::updateBufferPosition (juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& delayBuffer)
{
    auto bufferSize = buffer.getNumSamples();
    auto delayBufferSize = delayBuffer.getNumSamples();

    // wrap
    writePosition += bufferSize;
    writePosition %= delayBufferSize;
}

void DelayLine::setDelay (int time)
{
    delayTime = time;
}

juce::AudioBuffer<float>& DelayLine::getBuffer ()
{
    return delayBuffer;
}