/*
  ==============================================================================

    SingleCustomOsc.cpp
    Created: 1 Jul 2025 5:04:37pm
    Author:  Caleb Balboni

  ==============================================================================
*/

#include "SingleCustomOsc.h"


SingleCustomOsc::SingleCustomOsc(MySynthAudioProcessor& processor) {
    
    functionType.addItem("COSINE", 1);
    functionType.addItem("SINE", 2);
    functionType.setSelectedId(1);
    functionTypeLabel.setText("Function type", juce::dontSendNotification);
    functionTypeLabel.attachToComponent(&functionType, false);
    
    accuracy.setRange(1, 10);
    accuracyLabel.setText("accuracy", juce::dontSendNotification);
    accuracy.setSliderStyle(juce::Slider::Rotary);
    accuracyLabel.attachToComponent(&accuracy, false);
    
    multiplyVal.setRange(1.0f, 5.0f);
    multiplyVal.setSliderStyle(juce::Slider::Rotary);
    multiplyValLabel.setText("mul", juce::dontSendNotification);
    multiplyValLabel.attachToComponent(&multiplyVal, false);
    
    repeatNVal.setRange(1.0f, 3.0f);
    repeatNVal.setSliderStyle(juce::Slider::Rotary);
    repeatNValLabel.setText("N repeat's", juce::dontSendNotification);
    repeatNValLabel.attachToComponent(&repeatNVal, false);
    
    repeatXVal.setRange(1.0f, 3.0f);
    repeatXVal.setSliderStyle(juce::Slider::Rotary);
    repeatXValLabel.setText("X repeat's", juce::dontSendNotification);
    repeatXValLabel.attachToComponent(&repeatXVal, false);
    
    pitchVal.setRange(-2400.0f, 2400.0f);
    pitchVal.setSliderStyle(juce::Slider::Rotary);
    pitchValLabel.setText("pitch", juce::dontSendNotification);
    pitchValLabel.attachToComponent(&pitchVal, false);
    
    enabledLabel.setText("enable", juce::dontSendNotification);
    enabledLabel.attachToComponent(&enabled, false);
    
    /*
    juce::ComboBox functionType;
    juce::Slider accuracy;
    juce::Slider multiplyVal;
    juce::Slider repeatNVal;
    juce::Slider repeatXVal;
    juce::Slider pitchVal;
    juce::ToggleButton enabled;
     */
    
    addAndMakeVisible(&functionType);
    addAndMakeVisible(&accuracy);
    addAndMakeVisible(&multiplyVal);
    addAndMakeVisible(&repeatNVal);
    addAndMakeVisible(&repeatXVal);
    addAndMakeVisible(&pitchVal);
    addAndMakeVisible(&enabled);
    
    functionTypeAttachment =  new juce::AudioProcessorValueTreeState::ComboBoxAttachment(processor.apvts, "CUSTOMOSCFUNCTIONTYPE", functionType);
    
    accuracyAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.apvts, "CUSTOMOSCACCURACY", accuracy);
    
    multiplyValAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.apvts, "CUSTOMOSCMUL", multiplyVal);
    
    repeatNValAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.apvts, "CUSTOMOSCREPEATN", repeatNVal);
    
    repeatXValAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.apvts, "CUSTOMOSCREPEATX", repeatXVal);
    
    pitchValAttachment = new juce::AudioProcessorValueTreeState::SliderAttachment(processor.apvts, "CUSTOMOSCPITCH", pitchVal);
    
    enabledAttachment = new juce::AudioProcessorValueTreeState::ButtonAttachment(processor.apvts, "CUSTOMOSCENABLED", enabled);
}

SingleCustomOsc::~SingleCustomOsc() {
    delete accuracyAttachment;
    delete multiplyValAttachment;
    delete repeatNValAttachment;
    delete repeatXValAttachment;
    delete pitchValAttachment;
    delete enabledAttachment;
    delete functionTypeAttachment;
}

void SingleCustomOsc::resized()
{
    auto area = getLocalBounds().reduced (8);
    const int numCols     = 7;
    const int gap         = 10;
    const int labelHeight = 10;

    // compute the width of each column (all equal)
    int colWidth = 5 + (area.getWidth() - (numCols - 1) * gap) / numCols;

    // helper to pull off one column
    auto nextCol = [&]
    {
        auto c = area.removeFromLeft (colWidth);
        area.removeFromLeft (gap);
        return c;
    };

    // 1) Function type
    {
        auto c = nextCol();
        functionTypeLabel .setBounds (c.removeFromTop (labelHeight));
        functionType      .setBounds (c);
    }

    // 2) Accuracy
    {
        auto c = nextCol();
        accuracyLabel .setBounds (c.removeFromTop (labelHeight));
        accuracy      .setBounds (c);
    }

    // 3) Multiply
    {
        auto c = nextCol();
        multiplyValLabel .setBounds (c.removeFromTop (labelHeight));
        multiplyVal      .setBounds (c);
    }

    // 4) N repeats
    {
        auto c = nextCol();
        repeatNValLabel .setBounds (c.removeFromTop (labelHeight));
        repeatNVal      .setBounds (c);
    }

    // 5) X repeats
    {
        auto c = nextCol();
        repeatXValLabel .setBounds (c.removeFromTop (labelHeight));
        repeatXVal      .setBounds (c);
    }

    // 6) Pitch
    {
        auto c = nextCol();
        pitchValLabel .setBounds (c.removeFromTop (labelHeight));
        pitchVal      .setBounds (c);
    }

    // 7) Enable (take whatever space is left)
    {
        auto c = area;  // remaining area
        enabledLabel .setBounds (c.removeFromTop (labelHeight));
        enabled      .setBounds (c);
    }
}



