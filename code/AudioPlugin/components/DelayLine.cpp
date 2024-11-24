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
    auto readPosition = writePosition - (48000 * 0.5f); // magic number for getSampleRate() for now
    if (readPosition < 0) {
        readPosition += delayBufferSize;
    }
    auto g = 0.4f;
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

juce::AudioBuffer<float>& DelayLine::getBuffer ()
{
    return delayBuffer;
}

float DelayLine::getSample (int channel)
{
    return delayBuffer.getSample(channel, writePosition); //unsure
}