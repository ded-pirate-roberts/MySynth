/*
  ==============================================================================

    ReverbPanel.h
    Created: 2 Jul 2025 6:14:04pm
    Author:  Caleb Balboni

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

/// The panel that contains all the components for controlling the reverb.
class ReverbPanel : public juce::Component {
    
public:
    
    /// Constructor for the reverb object.
    /// - Parameter processor: The audio processor the components of the reverb will be attached to.
    ReverbPanel(MySynthAudioProcessor& processor);
    
    /// Destructor.
    ~ReverbPanel();
    
    /// Formats the components of the reverb.
    void resized() override;
private:
    juce::Slider roomSize;
    juce::Slider damping;
    juce::Slider wetLevel;
    juce::Slider dryLevel;
    juce::Slider width;
    juce::Slider freezeMode;
    juce::ToggleButton active;
    
    juce::Label reverbLabel;
    juce::Label roomSizeLabel;
    juce::Label dampingLabel;
    juce::Label wetLevelLabel;
    juce::Label dryLevelLabel;
    juce::Label widthLabel;
    juce::Label freezeModeLabel;
    juce::Label activeLabel;
    
    juce::AudioProcessorValueTreeState::SliderAttachment* roomSizeAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment* dampingAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment* wetLevelAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment* dryLevelAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment* widthAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment* freezeModeAttachment;
    juce::AudioProcessorValueTreeState::ButtonAttachment* activeAttachment;
    
};
