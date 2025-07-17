/*
  ==============================================================================

    ChorusPanel.h
    Created: 3 Jul 2025 8:36:47pm
    Author:  Caleb Balboni

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"


/// Represents the panel the holds the controls for the chorus.
class ChorusPanel : public juce::Component {
  
public:
    
    
    /// Constructor for the chorus panel.
    /// - Parameter processor: The processor all of the components of this panel will be attached to.
    ChorusPanel(MySynthAudioProcessor& processor);
    
    
    /// Destructor.
    ~ChorusPanel();
    
    
    /// Formats components on screen.
    void resized() override;
    
private:
    
    juce::Slider rate;
    juce::Slider depth;
    juce::Slider centreDelay;
    juce::Slider feedback;
    juce::Slider mix;
    juce::ToggleButton active;
    
    juce::Label chorusLabel;
    juce::Label rateLabel;
    juce::Label depthLabel;
    juce::Label centreDelayLabel;
    juce::Label feedbackLabel;
    juce::Label mixLabel;
    juce::Label activeLabel;
    
    juce::AudioProcessorValueTreeState::SliderAttachment* rateAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment* depthAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment* centreDelayAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment* feedbackAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment* mixAttachment;
    juce::AudioProcessorValueTreeState::ButtonAttachment* activeAttachment;
};
