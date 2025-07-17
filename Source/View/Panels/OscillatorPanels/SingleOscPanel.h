/*
  ==============================================================================

    SingleOscPanel.h
    Created: 29 Jun 2025 3:13:16pm
    Author:  Caleb Balboni

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"


/// Represents the panel that holds the controls for a single oscillator.
class SingleOscPanel : public juce::Component {
    
public:
    
    
    /// Constructor for this single oscillator.
    /// - Parameters:
    ///   - processor: The processor the sliders and combo box will attach to.
    ///   - oscName: The name of this oscillator (used to be indentified by the audio processor).
    SingleOscPanel(MySynthAudioProcessor& processor, std::string oscName);
    
    
    /// Destructor (removes pointers).
    ~SingleOscPanel();
    
    
    /// Formats objects in their proper parts.
    void resized() override;
    
    
private:
    // GUI components
    juce::ComboBox waveForm;
    juce::ToggleButton oscActive;
    juce::Slider oscPitch;
    
    // GUI Labels
    juce::Label waveFormLabel;
    juce::Label oscPitchLabel;
    
    // Attachments to apvts
    juce::AudioProcessorValueTreeState::ComboBoxAttachment* waveFormAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment* pitchAttachment;
    juce::AudioProcessorValueTreeState::ButtonAttachment* oscActiveAttachment;
    
};
