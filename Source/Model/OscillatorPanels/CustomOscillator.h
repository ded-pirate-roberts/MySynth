/*
  ==============================================================================

    CustomOscillator.h
    Created: 1 Jul 2025 6:02:24pm
    Author:  Caleb Balboni

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


/// A custom oscillator type that is based upon a fourier series.
class CustomOscillator : public juce::dsp::Oscillator<float> {
    
public:
    
    
    /// Constructor of this oscillator.
    CustomOscillator();
    
    
    /// Changes the parameters for how the wave type of this oscillator.
    /// - Parameters:
    ///   - typeIndex: The index of the base function type being used either 1 - COSINE, 2 - SINE.
    ///   - accuracy: Changes the amount of times this series will repeat.
    ///   - mulVal: Changes how multiplier of the repeat and x values.
    ///   - repeatX: The power that the x variable will be raised to.
    ///   - repeatN: The power that the n variable will be raised to.
    void changeFunctionType(int typeIndex, int accuracy, float mulVal, float repeatX, float repeatN);
    
    
    /// Changes if this oscillator is currently active.
    /// - Parameter isActive: true - oscillator active, false - oscillator inactive.
    void changeIsActive(bool isActive);
    
    
    /// Changes the detune of this oscillator.
    /// - Parameter pitchVal:The amount of cents to detune
    void changePitch(float pitchVal);
    
    
    /// Sets the frequency of this oscillator.
    /// - Parameter pitch: The frequency of the oscillator.
    void changeFrequency(float pitch);
    
    
    
    /// Adds the sounds of this oscillator to the audio block.
    /// - Parameter block: The audio block to be added to.
    void getNextAudioBlock (juce::dsp::AudioBlock<float>& block);
    
    
    /// Changes if this oscillator is active or not.
    bool isActive();
    
private:
    
    
    /// Creates the function for this oscillator based upon the input params.
    /// - Parameter x: The x value (phase) of the oscillator.
    float customFunction(float x);
    int typeIndex;
    int accuracy;
    float mulVal;
    float repeatX;
    float repeatN;
    float pitchVal;
    bool active;
};
