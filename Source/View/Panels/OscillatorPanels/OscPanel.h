/*
  ==============================================================================

    OscPanel.h
    Created: 24 Jun 2025 7:53:59pm
    Author:  Caleb Balboni

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "SingleOscPanel.h"
#include "AdsrPanel.h"


/// The panel that holds all of the oscillators in the 3 x 3 format.
class OscPanel : public juce::Component {
    
public:
    
    
    /// Constructs the oscillator panel.
    /// - Parameter processor: The audio processor that the sliders here will connect to.
    OscPanel(MySynthAudioProcessor& processor);
    
    
    /// Destructor (removes pointers).
    ~OscPanel();
private:
    
    
    /// Places objects in their proper spots.
    void resized() override;
    
    // OSCILLATOR CONTROLS
    
    SingleOscPanel osc11;
    SingleOscPanel osc12;
    SingleOscPanel osc13;
    
    SingleOscPanel osc21;
    SingleOscPanel osc22;
    SingleOscPanel osc23;
    
    SingleOscPanel osc31;
    SingleOscPanel osc32;
    SingleOscPanel osc33;
    AdsrPanel adsr;
    
    // FORMATTING FIELDS
    juce::GroupComponent basicControlGroup; // for the gain and oscillator wave type
    juce::GroupComponent adsrControlGroup; // for adsr components
    juce::Label panelName;
    
};
