/*
  ==============================================================================

    ReverbPanel.cpp
    Created: 2 Jul 2025 6:14:04pm
    Author:  Caleb Balboni

  ==============================================================================
*/

#include "ReverbPanel.h"

ReverbPanel::ReverbPanel(MySynthAudioProcessor& processor) {
    
    reverbLabel.setText("REVERB: ", juce::dontSendNotification);
    
    roomSize.setRange(0.0f, 1.0f);
    roomSize.setSliderStyle(juce::Slider::Rotary);
    roomSizeLabel.setText("size", juce::dontSendNotification);
    roomSizeLabel.attachToComponent(&roomSize, false);
    
    damping.setRange(0.0f, 1.0f);
    damping.setSliderStyle(juce::Slider::Rotary);
    dampingLabel.setText("damp", juce::dontSendNotification);
    dampingLabel.attachToComponent(&damping, false);
    
    
    wetLevel.setRange(0.0f, 1.0f);
    wetLevel.setSliderStyle(juce::Slider::Rotary);
    wetLevelLabel.setText("wet", juce::dontSendNotification);
    wetLevelLabel.attachToComponent(&wetLevel, false);
    
    
    dryLevel.setRange(0.0f, 1.0f);
    dryLevel.setSliderStyle(juce::Slider::Rotary);
    dryLevelLabel.setText("dry", juce::dontSendNotification);
    dryLevelLabel.attachToComponent(&dryLevel, false);
    
    width.setRange(0.0f, 1.0f);
    width.setSliderStyle(juce::Slider::Rotary);
    widthLabel.setText("width", juce::dontSendNotification);
    widthLabel.attachToComponent(&width, false);
    
    freezeMode.setRange(0.0f, 0.5f);
    freezeMode.setSliderStyle(juce::Slider::Rotary);
    freezeModeLabel.setText("freeze", juce::dontSendNotification);
    freezeModeLabel.attachToComponent(&freezeMode, false);
    
    activeLabel.setText("enable", juce::dontSendNotification);
    activeLabel.attachToComponent(&active, false);
    
    addAndMakeVisible(&roomSize);
    addAndMakeVisible(&damping);
    addAndMakeVisible(&wetLevel);
    addAndMakeVisible(&dryLevel);
    addAndMakeVisible(&width);
    addAndMakeVisible(&freezeMode);
    addAndMakeVisible(&active);
    
    addAndMakeVisible(&roomSizeLabel);
    addAndMakeVisible(&dampingLabel);
    addAndMakeVisible(&wetLevelLabel);
    addAndMakeVisible(&dryLevelLabel);
    addAndMakeVisible(&widthLabel);
    addAndMakeVisible(&freezeModeLabel);
    addAndMakeVisible(&reverbLabel);
    addAndMakeVisible(&activeLabel);
    
    roomSizeAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.apvts, "REVERBROOMSIZE", roomSize);
    dampingAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.apvts, "REVERBDAMPING", damping);
    wetLevelAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.apvts, "REVERBWETLEVEL", wetLevel);
    dryLevelAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.apvts, "REVERBDRYLEVEL", dryLevel);
    widthAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.apvts, "REVERBWIDTH", width);
    freezeModeAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.apvts, "REVERBFREEZEMODE", freezeMode);
    activeAttachment = new juce::AudioProcessorValueTreeState::ButtonAttachment(processor.apvts, "REVERBACTIVE", active);
}

ReverbPanel::~ReverbPanel() {
    delete roomSizeAttachment;
    delete dampingAttachment;
    delete wetLevelAttachment;
    delete dryLevelAttachment;
    delete widthAttachment;
    delete freezeModeAttachment;
    delete activeAttachment;
}

void ReverbPanel::resized() {
    reverbLabel.setBounds(0, 0, 50, 35);
    roomSize.setBounds(0, 50, 75, 40);
    damping.setBounds(75, 50, 75, 40);
    wetLevel.setBounds(150, 50, 75, 40);

    dryLevel.setBounds(0, 115, 75, 30);
    width.setBounds(75, 115, 75, 30);
    freezeMode.setBounds(150, 115, 75, 30);
    active.setBounds(235, 115, 50, 25);
    
}
