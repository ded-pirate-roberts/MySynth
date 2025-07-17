/*
  ==============================================================================

    OscillatorRow.h
    Created: 29 Jun 2025 8:50:24pm
    Author:  Caleb Balboni

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Oscillator.h"

/// Panel for one row of oscillators.
class OscillatorRow {
public:
    
    
    /// Constructor of this row of oscillators.
    OscillatorRow();
    
    /// Adds the sounds of each oscillator to the given block along with the effects that this block has enabled.
    /// - Parameter block: The audio block that is to be added to.
    void getNextAudioBlock (juce::dsp::AudioBlock<float>& block);
    
    
    /// Prepares the effects and oscillators of this given row.
    /// - Parameter spec: the specification of the incoming audio block.
    void prepareRow(juce::dsp::ProcessSpec spec);
    
    
    /// Turns of the adsr after the end of a note (this is autocalled by the synthvoice)
    void noteOff();
    
    
    /// Inits the oscillators with the proper midi note to play.
    /// - Parameter midiNote: the midi note as a midi note number (see JUCE docs).
    void startNote(int midiNote);
    
    
    /// Changes the pitch of one of the oscillators in this row.
    /// - Parameters:
    ///   - index: The index of the oscillator that is to be edited (1-3).
    ///   - cents: The amount of cents to detune.
    void changeDetuneCents(int index, float cents);
    
    
    /// Changes whether or not the given oscillator is active.
    /// - Parameters:
    ///   - index: The index of the oscillator to be edited (1-3).
    ///   - isActive: true - the oscillator is active, false - the oscillator is inactive
    void changeOscillatorIsActive(int index, bool isActive);
    
    
    /// Changes the waveform of the oscillator.
    /// - Parameters:
    ///   - index: The index of the oscillator to be edited (1-3).
    ///   - waveForm: the index of the wave form for the oscillator to now have. (Note to self: change this later to use ENUM)
    void changeOscillatorWaveForm(int index, int waveForm);
    
    // FOR THE FILTER
    
    
    /// Changes the cutoff frequency of the filter for this particular row.
    /// - Parameter newFrequency: The frequency for the cutoff to be set to.
    void changeFilterCutoff(float newFrequency);
    
    
    /// Changes the resonance value of the filter for this particular row.
    /// - Parameter resonance: The new resonance value for this filter.
    void changeFilterResonance(float resonance);
    
    
    /// Change the filter type of the filter for this row.
    /// - Parameter index: the index which represents the filter type (note to self: change to enum)
    void changeFilter(int index);
    
    // FOR THE LFO
    
    /// Changes the LFO depth for the oscillator in this row.
    /// - Parameter depth: The depth value of the oscillator.
    void changeLfoDepth(float depth);
    
    
    /// Changes the LFO frequency for this particular row.
    /// - Parameter frequency: The frequency of the oscillator.
    void changeLfoFrequency(float frequency);
    
    
    /// Changes the wave type of the LFO for this particular row.
    /// - Parameter index: The wave type of the LFO as an index.
    void changeLfoWaveType(float index);
    
    // FOR THE ADSR
    
    
    /// Changes the gain value for this particular row.
    /// - Parameter decibels: The new decibel value for this row.
    void changeGainDecibels(float decibels);
    
    
    /// Changes the attack value for the ADSR for this particular row.
    /// - Parameter attack: The attack value to be set.
    void changeAdsrAttack(float attack);
    
    
    /// Changes the decay value for the ADSR for this paticular row.
    /// - Parameter decay: The decay value to be set.
    void changeAdsrDecay(float decay);
    
    
    /// Changes the sustain value for the ADSR for this particular row.
    /// - Parameter sustain: The sustain value to be set.
    void changeAdsrSustain(float sustain);
    
    
    /// Changes the release value for the ADSR for this particular row.
    /// - Parameter release: The release value to be set.
    void changeAdsrRelease(float release);
    
private:
    
    
    /// updates the ADSR with values of the ADSR params variable.
    void updateADSR();
    
    
    /// Updates the Filters resonance, and cutoff, along with the interpolation of those values based upon the ADSR.
    void updateFilter();
    
    std::vector<std::unique_ptr<Oscillator>> oscillators;
    
    juce::dsp::Gain<float> gain;
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;
    
    juce::dsp::StateVariableTPTFilter<float> filter;
    float filterCutoff;
    float filterResonance;
    bool filterActive;
    
    std::unique_ptr<Oscillator> lfo;
    float lfoDepth;
    float lfoFrequency;
    float baseFrequency;
    
};
