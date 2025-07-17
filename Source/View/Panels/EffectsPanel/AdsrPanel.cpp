/*
  ==============================================================================

    AdsrPanel.cpp
    Created: 6 Jul 2025 2:26:35pm
    Author:  Caleb Balboni

  ==============================================================================
*/

#include "AdsrPanel.h"

AdsrPanel::AdsrPanel(MySynthAudioProcessor& processor, std::string name) {
    
    // gain
    gainSlider.setName("GAIN");
    gainSlider.setRange(-50.0, 6.0);
    gainSlider.setValue(1.0f);
    gainLabel.setText("gain", juce::dontSendNotification);
    gainLabel.attachToComponent(&gainSlider, false);

    // ADSR:
    
    // attack
    attackSlider.setName("ATTACK");
    attackSlider.setSliderStyle(juce::Slider::Rotary);
    attackSlider.setRange(0.0f, 5.0f);
    attackLabel.setText("attack", juce::dontSendNotification);
    attackLabel.attachToComponent(&attackSlider, false);
    
    // release
    releaseSlider.setName("RELEASE");
    releaseSlider.setSliderStyle(juce::Slider::Rotary);
    releaseSlider.setRange(0.0f, 5.0f);
    releaseLabel.setText("release", juce::dontSendNotification);
    releaseLabel.attachToComponent(&releaseSlider, false);
    
    // decay
    decaySlider.setName("DECAY");
    decaySlider.setSliderStyle(juce::Slider::Rotary);
    decaySlider.setRange(0.0f, 5.0f);
    decayLabel.setText("decay", juce::dontSendNotification);
    decayLabel.attachToComponent(&decaySlider, false);
    
    // sustain
    sustainSlider.setName("SUSTAIN");
    sustainSlider.setSliderStyle(juce::Slider::Rotary);
    sustainSlider.setRange(0.0f, 5.0f);
    sustainLabel.setText("sustain", juce::dontSendNotification);
    sustainLabel.attachToComponent(&sustainSlider, false);
    
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(releaseSlider);
    addAndMakeVisible(decaySlider);
    addAndMakeVisible(sustainSlider);
    
    releaseAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.apvts, juce::String (name + "RELEASE"), releaseSlider);
    sustainAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.apvts, juce::String (name + "SUSTAIN"), sustainSlider);
    attackAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.apvts, juce::String (name + "ATTACK"), attackSlider);
    decayAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.apvts, juce::String (name + "DECAY"), decaySlider);
    gainAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.apvts, juce::String (name + "GAIN"), gainSlider);
}

void AdsrPanel::resized()
{
    auto area  = getLocalBounds().reduced(20);
    const int rowH = 30;
    const int gap  = 20;

    // ─── left half: gain ───────────────────────────────────
    auto gainArea = area.removeFromLeft(area.getWidth() / 2);
    gainSlider.setBounds(gainArea.removeFromTop(rowH));

    // ─── right half: ADSR ───────────────────────────────────
    auto adsrArea = area; // what's left after removing gain
    // split right half into two columns for ADSR
    auto c1 = adsrArea.removeFromLeft(adsrArea.getWidth() / 2);
    auto c2 = adsrArea;

    // Attack / Sustain in column 1
    attackSlider.setBounds  (c1.removeFromTop(rowH));
    c1.removeFromTop(gap);
    sustainSlider.setBounds (c1.removeFromTop(rowH));

    // Decay / Release in column 2
    decaySlider.setBounds   (c2.removeFromTop(rowH));
    c2.removeFromTop(gap);
    releaseSlider.setBounds (c2.removeFromTop(rowH));
}
