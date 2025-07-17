/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SynthVoice.h"
//==============================================================================
MySynthAudioProcessorEditor::MySynthAudioProcessorEditor (MySynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), oscControls(audioProcessor),
                                                   filterControls1(audioProcessor, "OSC1"),
                                                   filterControls2(audioProcessor, "OSC2"),
                                                   filterControls3(audioProcessor, "OSC3"),
                                                   lfoControls(audioProcessor),
                                                   customOscControls(audioProcessor),
                                                   reverbControls(audioProcessor),
                                                   chorusControls(audioProcessor)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible(oscControls);
    addAndMakeVisible(filterControls1);
    addAndMakeVisible(filterControls2);
    addAndMakeVisible(filterControls3);
    addAndMakeVisible(lfoControls);
    addAndMakeVisible(customOscControls);
    addAndMakeVisible(reverbControls);
    addAndMakeVisible(chorusControls);

    setSize (1100, 775);
}

MySynthAudioProcessorEditor::~MySynthAudioProcessorEditor()
{
}

//==============================================================================
void MySynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (12.0f));
}

void MySynthAudioProcessorEditor::resized()
{
    oscControls.setBounds(20,50, 450, 600);
    reverbControls.setBounds(700, 30, 300, 200);
    chorusControls.setBounds(700, 575, 300, 200);
    customOscControls.setBounds(20,650, 600,75);
    lfoControls.setBounds(500, 65, 150, 150);
    filterControls1.setBounds(500, 155, 600, 200);
    filterControls2.setBounds(500, 295, 600, 200);
    filterControls3.setBounds(500, 455, 600, 200);
}






