/*
  ==============================================================================

    FilterPanel.cpp
    Created: 26 Jun 2025 11:48:17pm
    Author:  Caleb Balboni

  ==============================================================================
*/

#include "FilterPanel.h"


Filter::Filter(MySynthAudioProcessor& processor, std::string oscRow) : adsrControls1(processor, oscRow){
    
    setSize(100, 250);
    // NAME
    //filterLabel.setText("Filters:", juce::dontSendNotification);
    
    // FILTER
    filterType.setName("FILTERTYPE");
    filterType.addItem("NONE", 1);
    filterType.addItem("LOWPASS", 2);
    filterType.addItem("HIGHPASS", 3);
    filterType.addItem("BANDPASS", 4);
    filterType.setSelectedId(1);
    filterTypeLabel.setText("Filter Type:", juce::dontSendNotification);
    filterTypeLabel.attachToComponent(&filterType, false);
    
    // CUTOFF SLIDER
    cutoffSlider.setName("CUTOFFSLIDER");
    cutoffSlider.setSliderStyle(juce::Slider::Rotary);
    cutoffSlider.setRange(0.0f, 20000.0f);
    cutoffLabel.setText("Cutoff: ", juce::dontSendNotification);
    cutoffLabel.attachToComponent(&cutoffSlider, false);
    
    // RESONANCE SLIDER
    resonanceSlider.setName("RESONANCESLIDER");
    resonanceSlider.setSliderStyle(juce::Slider::Rotary);
    resonanceSlider.setRange(0.1f, 12.0f);
    resonanceLabel.setText("Resonance: ", juce::dontSendNotification);
    resonanceLabel.attachToComponent(&resonanceSlider, false);
    
    addAndMakeVisible(&filterType);
    addAndMakeVisible(&cutoffSlider);
    addAndMakeVisible(&resonanceSlider);
    addAndMakeVisible(adsrControls1);
    filterLabel   .setInterceptsMouseClicks (false, false);
    cutoffLabel   .setInterceptsMouseClicks (false, false);
    resonanceLabel.setInterceptsMouseClicks (false, false);
    
    filterTypeAttachment = new juce::AudioProcessorValueTreeState::ComboBoxAttachment(processor.apvts,
                                                             juce::String (oscRow + "FILTERTYPE"), filterType);
    cutoffAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.apvts,
                                                             juce::String (oscRow + "CUTOFF"), cutoffSlider);
    resonanceAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.apvts,
                                                             juce::String (oscRow + "RESONANCE"), resonanceSlider);
    
}

Filter::~Filter() {
    delete cutoffAttachment;
    delete resonanceAttachment;
    delete filterTypeAttachment;
}

void Filter::resized() {
    //filterLabel.setBounds(0,0, 100,50);
    
    filterType.setBounds(0, 75, 75, 40);
    cutoffSlider.setBounds(90, 75, 100, 50);
    resonanceSlider.setBounds(190, 75, 100, 50);
    adsrControls1.setBounds(300, 50, 300, 200);
}

