# Dual-Reverb
A reverb plugin created with JUCE.

## Design and Information
Dual-Reverb uses four Feedback Comb Filters in parallel with two Allpass Filters in series. They are implemented with delay lines tuned to prime numbers to prevent regular peaks from being discerned in the output signal.

## Getting Started
This program uses [CMake](https://cmake.org/) to compile. It assumes that the latest version of [JUCE](https://github.com/juce-framework/JUCE) is cloned to a directory under ```/code/AudioPlugin```. Once installed, it can be compiled using ```make```.
