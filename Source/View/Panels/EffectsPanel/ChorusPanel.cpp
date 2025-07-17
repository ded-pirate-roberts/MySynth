/*
  ==============================================================================

    ChorusPanel.cpp
    Created: 3 Jul 2025 8:36:47pm
    Author:  Caleb Balboni

  ==============================================================================
*/

#include "ChorusPanel.h"

ChorusPanel::ChorusPanel(MySynthAudioProcessor& processor) {
    
    chorusLabel.setText("Chorus", juce::dontSendNotification);
    
    rate.setRange(0.0f, 10.0f);
    rate.setSliderStyle(juce::Slider::Rotary);
    rateLabel.setText("rate", juce::dontSendNotification);
    rateLabel.attachToComponent(&rate, false);
    
    depth.setRange(0.0f, 1.0f);
    rate.setSliderStyle(juce::Slider::Rotary);
    depthLabel.setText("depth", juce::dontSendNotification);
    depthLabel.attachToComponent(&depth, false);
    
    centreDelay.setRange(0.0f, 1000.0f);
    rate.setSliderStyle(juce::Slider::Rotary);
    centreDelayLabel.setText("delay", juce::dontSendNotification);
    centreDelayLabel.attachToComponent(&centreDelay, false);
    
    feedback.setRange(-1.0f, 1.0f);
    rate.setSliderStyle(juce::Slider::Rotary);
    feedbackLabel.setText("feedback", juce::dontSendNotification);
    feedbackLabel.attachToComponent(&feedback, false);
    
    mix.setRange(0.0f, 1.0f);
    rate.setSliderStyle(juce::Slider::Rotary);
    mixLabel.setText("mix", juce::dontSendNotification);
    mixLabel.attachToComponent(&mix, false);
    
    activeLabel.setText("enable", juce::dontSendNotification);
    activeLabel.attachToComponent(&active, false);
    
    /*
     juce::Slider rate;
     juce::Slider depth;
     juce::Slider centreDelay;
     juce::Slider feedback;
     juce::Slider mix;
     juce::ToggleButton active;
     */
    
    addAndMakeVisible(rate);
    addAndMakeVisible(depth);
    addAndMakeVisible(centreDelay);
    addAndMakeVisible(feedback);
    addAndMakeVisible(mix);
    addAndMakeVisible(active);
    addAndMakeVisible(chorusLabel);
    
    rateAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.apvts, "CHORUSRATE", rate);
    depthAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.apvts, "CHORUSDEPTH", depth);
    centreDelayAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.apvts, "CHORUSCENTREDELAY", centreDelay);
    feedbackAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.apvts, "CHORUSFEEDBACK", feedback);
    mixAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.apvts, "CHORUSMIX", mix);
    activeAttachment = new juce::AudioProcessorValueTreeState::ButtonAttachment(processor.apvts, "CHORUSACTIVE", active);
}

ChorusPanel::~ChorusPanel() {
    delete rateAttachment;
    delete depthAttachment;
    delete centreDelayAttachment;
    delete feedbackAttachment;
    delete mixAttachment;
    delete activeAttachment;
}

void ChorusPanel::resized() {
    chorusLabel.setBounds(0, 0, 50, 30);
    depth.setBounds(0, 50, 75, 40);
    centreDelay.setBounds(75, 50, 75, 40);
    feedback.setBounds(150, 50, 75, 40);

    mix.setBounds(0, 115, 75, 30);
    rate.setBounds(75, 115, 75, 35);
    active.setBounds(150, 115, 75, 30);
}
