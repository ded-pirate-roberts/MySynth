/*
  ==============================================================================

    FilterPanel.h
    Created: 26 Jun 2025 11:48:17pm
    Author:  Caleb Balboni

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "AdsrPanel.h"


/// Represents the Panel for the controls of one of the filters.
class Filter : public juce::Component {
    
public:
    
    
    /// Constructs the filter panel object.
    /// - Parameters:
    ///   - processor: The audio processor that the components of the panel will be attached to.
    ///   - oscRow: The row of oscillators this Filter is currently attached to.
    Filter(MySynthAudioProcessor& processor, std::string oscRow);
    
    
    /// Destructor.
    ~Filter();
    juce::Slider cutoffSlider;
    juce::Slider resonanceSlider;
    juce::ComboBox filterType;
    AdsrPanel adsrControls1;
    
    juce::Label filterLabel;
    juce::Label cutoffLabel;
    juce::Label resonanceLabel;
    juce::Label filterTypeLabel;
    
    
    /// Formats all components.
    void resized() override;
    
private:
    
    juce::AudioProcessorValueTreeState::ComboBoxAttachment* filterTypeAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment* cutoffAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment* resonanceAttachment;
};
