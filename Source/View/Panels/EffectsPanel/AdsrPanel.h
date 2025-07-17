/*
  ==============================================================================

    AdsrPanel.h
    Created: 6 Jul 2025 2:26:35pm
    Author:  Caleb Balboni

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"


/// A Panel in which the user can control the ADSR for  objects like filters ect.
class AdsrPanel : public juce::Component {
    
public:
    
    
    /// Constructor for the ADSR Panel.
    /// - Parameters:
    ///   - processor: The processor that the these sliders will be connected to.
    ///   - name: The name that will be associted with the adsr panel for identification by the apvts.
    AdsrPanel(MySynthAudioProcessor& processor, std::string name);
    
    
    /// Formats the panel.
    void resized() override;
    
private:
    // ADSR STUFF
    juce::Slider releaseSlider;
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider gainSlider;
    
    juce::Label sustainLabel;
    juce::Label decayLabel;
    juce::Label releaseLabel;
    juce::Label attackLabel;
    juce::Label gainLabel;
    
    juce::AudioProcessorValueTreeState::SliderAttachment* releaseAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment* attackAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment* sustainAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment* decayAttachment;
    juce::AudioProcessorValueTreeState::SliderAttachment* gainAttachment;
};
