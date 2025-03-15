# Dual-Reverb
A reverb plugin created with JUCE.

## Design and Information
Dual-Reverb uses four Feedback Comb Filters in parallel to reverberate a provided input signal. They are implemented with delay lines tuned to prime numbers to prevent regular peaks from being discerned in the output signal. A Convolution Reverberator implemented via JUCE's DSP library is provided as a baseline to compare against.

## Getting Started
This program uses [CMake](https://cmake.org/) to compile. It assumes that the latest version of [JUCE](https://github.com/juce-framework/JUCE) is cloned to a directory under ```../code/AudioPlugin```. Once installed, it can be compiled using ```make```.
