# Dual-Reverb
A reverb plugin made with JUCE.

## Information
Dual-Reverb was written in an attempt to understand audio programming and plugin development. The program, in combination with its associated paper, comprise my senior thesis which I completed Spring 2025.
## Getting Started
This program uses [CMake](https://cmake.org/) to compile. It assumes that the latest version of [JUCE](https://github.com/juce-framework/JUCE) is cloned to a directory under ```../code/AudioPlugin/JUCE```. Once installed, it can be compiled with:

1. `cd Dual-Reverb`
2. `mkdir build && cd build`
3. `cmake -DCMAKE_BUILD_TYPE=Release ..`
4. `make`

## Recommended Citation
``` 
Sieber, William A., "Development of Artificial Reverberation in C++" (2025). Senior Independent Study Theses. Paper 11318.
https://openworks.wooster.edu/independentstudy/11318 
```
