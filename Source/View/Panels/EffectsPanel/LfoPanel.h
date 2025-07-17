/*
  ==============================================================================

    LfoPanel.h
    Created: 30 Jun 2025 10:23:05pm
    Author:  Caleb Balboni

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"


/// The panel that contains all the components for controlling the LFO.
class LfoPanel : public juce::Component {
public:
    
    
    /// Constructor for the LFO panel.
    /// - Parameter processor: The audio processor that the LFO will be attached to.
    LfoPanel(MySynthAudioProcessor& processor);
    
    
    /// Destructor for the LFO.
    ~LfoPanel();
    
private:
    
    /// Formats components of LFO.
    void resized() override;
    
    juce::ComboBox lfoWaveType;
    juce::Slider lfoFrequency;
    juce::Slider lfoDepth;
    
    juce::GroupComponent lfoGroupControls;
    
    juce::Label lfoWaveTypeLabel;
    juce::Label lfoFrequencyLabel;
    juce::Label lfoDepthLabel;
    
    juce::AudioProcessorValueTreeState::ComboBoxAttachment* lfoWaveTypeAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment* frequencyAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment* depthAttachment;
};
