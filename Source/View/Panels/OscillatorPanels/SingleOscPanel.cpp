/*
  ==============================================================================

    SingleOscPanel.cpp
    Created: 29 Jun 2025 3:13:16pm
    Author:  Caleb Balboni

  ==============================================================================
*/

#include "SingleOscPanel.h"

// SETS UP THE SINGLE OSCILLATOR PANEL THIS PANEL HAS ONE OSCILLATOR WITH
// A WAVE SELECTOR A PITCH SLIDER AND AN ON OFF TOGGLE SWITCH
SingleOscPanel::SingleOscPanel(MySynthAudioProcessor& processor, std::string oscName) {
    
    waveForm.setName("WAVESELECTOR1");
    waveForm.addItem("SINE", 1);
    waveForm.addItem("SQUARE", 2);
    waveForm.addItem("SIGMOID", 3);
    waveForm.addItem("SAW", 4);
    waveForm.addItem("BITSAW", 5);
    waveForm.addItem("HALFSINE", 6);
    waveForm.addItem("TRIANGLE", 7);
    waveForm.addItem("DOUBLESINE", 8);
    waveForm.addItem("DOUBLECOSINE", 9);
    waveForm.addItem("WHITENOISE", 10);
    waveForm.addItem("POWERSINE", 11);
    waveForm.addItem("SINCWAVE", 12);
    waveForm.addItem("SOFTSQUARE", 13); 
    waveForm.addItem("POLYWAVE", 14);
    waveForm.addItem("HYPERTAN", 15);
    waveFormLabel.setText("wave", juce::dontSendNotification);
    waveFormLabel.attachToComponent (&waveForm, false);
    waveFormLabel.setJustificationType (juce::Justification::centred);
    waveFormAttachment = new juce::AudioProcessorValueTreeState::ComboBoxAttachment(processor.apvts, juce::String (oscName + "WAVETYPE"), waveForm);
    
    oscActive.setName("OSCACTIVE");
    oscActive.setButtonText("enable osc");
    oscActive.setToggleState(false, juce::dontSendNotification);
    oscActiveAttachment = new juce::AudioProcessorValueTreeState::ButtonAttachment(processor.apvts, juce::String (oscName + "ACTIVE"), oscActive);
    
    oscPitch.setName("OSCPITCH");
    oscPitch.setRange(-2400.0f, 2400.0f);
    oscPitch.setSliderStyle(juce::Slider::Rotary);
    oscPitchLabel.setText("pitch", juce::dontSendNotification);
    oscPitchLabel.attachToComponent(&oscPitch, true);
    
    pitchAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.apvts, juce::String (oscName + "PITCH"), oscPitch);
    
    addAndMakeVisible(&waveForm);
    addAndMakeVisible(&oscActive);
    addAndMakeVisible(&oscPitch);
}

SingleOscPanel::~SingleOscPanel() {
    delete waveFormAttachment;
    delete pitchAttachment;
    delete oscActiveAttachment;
}

void SingleOscPanel::resized()
{
    auto area    = getLocalBounds().reduced (4);
    const int rowH    = 22;
    const int spacing = 6;

    waveForm.setBounds (area.removeFromTop (rowH));
    area.removeFromTop (spacing);
    oscActive.setBounds (area.removeFromTop (rowH));
    area.removeFromTop (spacing);

    int knob = std::min (area.getWidth(), area.getHeight());  
    oscPitch.setBounds (area.getCentreX() - knob / 2,
                        area.getCentreY() - knob / 2,
                        knob, knob);
}


