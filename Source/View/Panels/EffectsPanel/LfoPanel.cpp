/*
  ==============================================================================

    LfoPanel.cpp
    Created: 30 Jun 2025 10:23:05pm
    Author:  Caleb Balboni

  ==============================================================================
*/

#include "LfoPanel.h"


LfoPanel::LfoPanel(MySynthAudioProcessor& processor) {
    lfoWaveType.setName("WAVESELECTOR1");
    lfoWaveType.addItem("SINE", 1);
    lfoWaveType.addItem("SQUARE", 2);
    lfoWaveType.addItem("SIGMOID", 3);
    lfoWaveType.addItem("SAW", 4);
    lfoWaveType.addItem("BITSAW", 5);
    lfoWaveType.addItem("HALFSINE", 6);
    lfoWaveType.addItem("TRIANGLE", 7);
    lfoWaveType.addItem("DOUBLESINE", 8);
    lfoWaveType.addItem("DOUBLECOSINE", 9);
    lfoWaveTypeLabel.setText("Lfo", juce::dontSendNotification);
    lfoWaveTypeLabel.attachToComponent (&lfoWaveType, false);
    lfoWaveTypeLabel.setJustificationType (juce::Justification::centred);
    lfoWaveTypeAttachment = new juce::AudioProcessorValueTreeState::ComboBoxAttachment(processor.apvts, "LFO1WAVETYPE", lfoWaveType);
    
    lfoFrequency.setName("FREQUENCYSLIDER1");
    lfoFrequency.setSliderStyle(juce::Slider::Rotary);
    lfoFrequency.setRange(0.0f, 20.0f);
    lfoFrequencyLabel.setText("Lfo frequency", juce::dontSendNotification);
    lfoFrequencyLabel.attachToComponent(&lfoFrequency, false);
    
    frequencyAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.apvts, "LFO1FREQUENCY", lfoFrequency);
    
    lfoDepth.setName("FREQUENCYSLIDER1");
    lfoDepth.setSliderStyle(juce::Slider::Rotary);
    lfoDepth.setRange(0.0f, 1.0f);
    lfoDepthLabel.setText("Lfo Depth", juce::dontSendNotification);
    lfoDepthLabel.attachToComponent(&lfoDepth, false);
    
    depthAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.apvts, "LFO1DEPTH", lfoDepth);
    
    addAndMakeVisible(&lfoWaveType);
    addAndMakeVisible(&lfoFrequency);
    addAndMakeVisible(&lfoDepth);
}

LfoPanel::~LfoPanel() {
    delete depthAttachment;
    delete frequencyAttachment;
    delete lfoWaveTypeAttachment;
}

void LfoPanel::resized() {
    int lfoWaveH = 35;
    int lfoWaveW = 100;
    
    int sliderH = 50;
    int sliderW = 75;
    
    lfoWaveType.setBounds(0,0, lfoWaveW, lfoWaveH);
    lfoFrequency.setBounds(0, 60, sliderW, sliderH);
    lfoDepth.setBounds(75, 60, sliderW, sliderH);
}
