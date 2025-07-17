/*
  ==============================================================================

    SingleCustomOsc.h
    Created: 1 Jul 2025 5:04:37pm
    Author:  Caleb Balboni

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"


/// A Panel for the custom oscillator which is based upon a repeating series function. Lets the user select the params
/// for this oscillator.
class SingleCustomOsc : public juce::Component {
    public:
    
    /// Constructor for the Custom Oscillator Panel.
    /// - Parameter processor: The audio processor (controller) that the buttons types will connent to.
    SingleCustomOsc(MySynthAudioProcessor& processor);
    
    
    /// Destructor for this object (remove pointers to apvts).
    ~SingleCustomOsc();
    
    
    /// Formats the Panel.
    void resized() override;
    
    private:
    juce::ComboBox functionType;
    juce::Slider accuracy;
    juce::Slider multiplyVal;
    juce::Slider repeatNVal;
    juce::Slider repeatXVal;
    juce::Slider pitchVal;
    juce::ToggleButton enabled;
    
    
    juce::Label functionTypeLabel;
    juce::Label accuracyLabel;
    juce::Label multiplyValLabel;
    juce::Label repeatNValLabel;
    juce::Label repeatXValLabel;
    juce::Label pitchValLabel;
    juce::Label enabledLabel;
    
    juce::AudioProcessorValueTreeState::ComboBoxAttachment* functionTypeAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment* accuracyAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment* multiplyValAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment* repeatNValAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment* repeatXValAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment* pitchValAttachment;
    juce::AudioProcessorValueTreeState::ButtonAttachment* enabledAttachment;
    
};
    
