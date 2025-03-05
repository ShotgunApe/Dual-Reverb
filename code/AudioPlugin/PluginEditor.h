#pragma once

#include "PluginProcessor.h"
#include "juce_audio_formats/juce_audio_formats.h"
#include "juce_gui_basics/juce_gui_basics.h"

//==============================================================================
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor,
                                              private juce::Slider::Listener
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void sliderValueChanged (juce::Slider* slider) override;
    void styleMenuChanged ();
    void openSample ();

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& processorRef;

    // Formatmanager keeps track of opened files in the audio plugin
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::FileChooser> chooser;

    // Frontend parameters
    juce::Slider roomSize;
    juce::Slider mix;
    juce::Slider gain;
    juce::Label textLabel { {}, "v0.02" };
    juce::ComboBox styleMenu;
    juce::TextButton loadSample;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
