#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);

    // UI EDITING!
    roomSize.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    roomSize.setRange (19350, 1935000, 1.0);
    roomSize.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    roomSize.setPopupDisplayEnabled (true, false, this);
    roomSize.setTextValueSuffix (" Room Size");
    roomSize.setValue (1.0);

    mix.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    mix.setRange (0.0, 127.0, 1.0);
    mix.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    mix.setPopupDisplayEnabled (true, false, this);
    mix.setTextValueSuffix (" Wet/Dry");
    mix.setValue (1.0);

    gain.setSliderStyle (juce::Slider::LinearVertical);
    gain.setRange (0.0f, 1.0f, 0.01f);
    gain.setTextBoxStyle (juce::Slider::NoTextBox, false, 90, 0);
    gain.setPopupDisplayEnabled (true, false, this);
    gain.setTextValueSuffix (" Gain");
    gain.setValue (0.8f);

    loadSample.setButtonText ("Load IR...");
    
    // Load File from Button try #1
    loadSample.onClick = [this] { openSample();};

    // Stylemenu options for selecting reverb type
    styleMenu.addItem("Comb + Allpass", 1);
    styleMenu.addItem("Convolution (DSP Module)", 2);

    // Make UI Visible
    addAndMakeVisible (&roomSize);
    addAndMakeVisible (&mix);
    addAndMakeVisible (&gain);
    addAndMakeVisible (&textLabel);
    addAndMakeVisible (&styleMenu);
    addAndMakeVisible (&loadSample);

    setSize (450, 200);

    // ADD LISTENERS FOR UI
    gain.addListener (this);
    roomSize.addListener (this);

    styleMenu.onChange = [this] { styleMenuChanged(); };
    styleMenu.setSelectedId (1);

    // boilerplate
    formatManager.registerBasicFormats();

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

    auto area = getLocalBounds();

    roomSize.setBounds (50, 100, 80, 80);
    mix.setBounds (150, 100, 80, 80);
    gain.setBounds (400, 25, 50, 150);
    textLabel.setBounds (10, 10, 300, 10);
    styleMenu.setBounds (50, 65, 350, 25);
    loadSample.setBounds (220, 30, 130, 25);
}

void AudioPluginAudioProcessorEditor::sliderValueChanged (juce::Slider* slider) 
{
    processorRef.proc_gain = gain.getValue();
    processorRef.proc_roomsize = roomSize.getValue();
}

void AudioPluginAudioProcessorEditor::styleMenuChanged ()
{
    processorRef.proc_reverb_type = styleMenu.getSelectedId();
}

void AudioPluginAudioProcessorEditor::openSample () {
    
    chooser = std::make_unique<juce::FileChooser>("Choose an Impulse Response", juce::File::getSpecialLocation(juce::File::userDesktopDirectory), "*.wav");
    auto fileFlags = juce::FileBrowserComponent::openMode;

    chooser->launchAsync(fileFlags, [this](const juce::FileChooser& opened)
    {
        juce::File chosenFile = opened.getResult();
        // Code to handle chosen files data

    });
}