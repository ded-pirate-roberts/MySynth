/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "FilterPanel.h"
#include "OscPanel.h"
#include "LfoPanel.h"
#include "SingleCustomOsc.h"
#include "ReverbPanel.h"
#include "ChorusPanel.h"

//==============================================================================


/// Main view component that formats all other parts of the view.
class MySynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    MySynthAudioProcessorEditor (MySynthAudioProcessor&);
    ~MySynthAudioProcessorEditor() override;
    MySynthAudioProcessor& audioProcessor;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OscPanel oscControls;
    Filter filterControls1;
    Filter filterControls2;
    Filter filterControls3;
    LfoPanel lfoControls;
    SingleCustomOsc customOscControls;
    ReverbPanel reverbControls;
    ChorusPanel chorusControls;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MySynthAudioProcessorEditor)
};

