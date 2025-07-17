/*
  ==============================================================================

    Oscillator.h
    Created: 25 Jun 2025 5:26:37pm
    Author:  Caleb Balboni

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


/// Represents a basic oscillator.
class Oscillator : public juce::dsp::Oscillator<float> {
public:
    
    
    /// Constructor for the oscillator object.
    Oscillator();
    
    /// Returns whether or not this oscillator is active.
    bool isActive();
    
    
    /// Sets this oscillator as active or not.
    /// - Parameter isActive: true - sets the oscillator as active, false - sets the oscillator as non active.
    void setActive(bool isActive);
    
    
    /// Changes the wave type of this oscillator.
    /// - Parameter type: The index of the type of wave to change this oscillator to (change index to enum)
    void changeWaveType(int type);
    
    
    /// Changes the frequency of this oscillator.
    /// - Parameter hz: The new frequency of the oscillator in hz.
    void changeFrequency (float hz);
    
    
    /// Changes the amount of cents to detune this oscillator.
    /// - Parameter cents: The amount of cents to detune.
    void setDetuneCents(float cents);
    
    
    /// Process this audio block and adds its data to the audio block.
    /// - Parameter block: The block of data to be added to.
    void getNextAudioBlock (juce::dsp::AudioBlock<float>& block);
    
    
    /// Processes the FM modulator for this oscillator.
    /// - Parameter block: The block to be processed
    void processFmOsc (juce::dsp::AudioBlock<float>& block);
    
    
    /// Updates the FM oscillators freqeuncy and depth.
    /// - Parameters:
    ///   - freq: The frequency of the FM oscillator to be set.
    ///   - depth: The depth of the FM oscillator to be set.
    void updateFm (const float freq, const float depth);
private:
    
    // ALL FUNCTIONS REPRESENTING THE WAVES THIS OSCILLATOR CAN HAVE
    double sigmoid(float x);
    float saw (float x);
    float tri (float x);
    float doubleSine(float x);
    float doubleCos(float x);
    float whiteNoise(float x);
    float powerSine(float x);
    float sincWave(float x);
    float polyWave(float x);
    float hyperbolicTan(float x);
    float softSquare(float x);
    float halfSine(float x);
    float fmMod;
    float fmDepth;
    float detuneCents;
    bool active;
};
