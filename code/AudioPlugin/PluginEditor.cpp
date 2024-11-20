#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 300);


    // these define the parameters of our slider object
    volume.setSliderStyle (juce::Slider::LinearBarVertical);
    volume.setRange (0.0, 127.0, 1.0);
    volume.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    volume.setPopupDisplayEnabled (true, false, this);
    volume.setTextValueSuffix (" Volume");
    volume.setValue(1.0);


    room_size.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    room_size.setRange (0.0, 127.0, 1.0);
    room_size.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    room_size.setPopupDisplayEnabled (true, false, this);
    room_size.setTextValueSuffix (" Room Size");
    room_size.setValue(1.0);
 
    addAndMakeVisible (&volume);
    addAndMakeVisible (&room_size);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    // g.drawFittedText ("Hello World again!", getLocalBounds(), juce::Justification::centred, 1);
}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    volume.setBounds (40, 30, 20, getHeight() - 60);
    room_size.setBounds (200, 200, 80, 80);
}
