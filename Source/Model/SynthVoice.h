/*
  ==============================================================================

    SynthVoice.h
    Created: 22 Jun 2025 2:26:16pm
    Author:  caleb

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SynthSound.h"
#include "OscillatorRow.h"
#include "CustomOscillator.h"
#include <vector>


/// Represents a voice within the synth.
class SynthVoice : public juce::SynthesiserVoice {
public:

    
    /// Constructor for the synth voice object.
    SynthVoice();
    
    
    /// Destructor for the synth voice object.
    ~SynthVoice();
    
    // ALL FUNCTIONS THAT ARE PART OF THE juce::SynthesiserVoice class, see JUCE docs.
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void stopNote(float velo, bool allowTailOff) override;
    void startNote(int midiNoteNum, float velo, juce::SynthesiserSound* sound, int currentPitchWheelPos) override;
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    
    // CHANGE OSCILLATOR PARAMS (the row and column represent where the oscillator is in the 3x3 grid)
    
    
    /// Changes the wave type of the oscillator at the given row and column.
    /// - Parameters:
    ///   - row: The row the oscillator is in.
    ///   - column: The column the oscillator is in.
    ///   - type: The wave type as a number.
    void changeOscillatorWaveType(int row, int column, int type);
    
    
    /// Changes if the oscillator is active at the given row and column.
    /// - Parameters:
    ///   - row: The row the oscillator is in.
    ///   - column: The column the oscillator is in.
    ///   - isActive: True - makes oscillator active, false - makes oscillator inactive.
    void changeOscillatorIsActive(int row, int column, bool isActive);
    
    
    /// Changes the amount of detune cents of the oscillator at the given row and column.
    /// - Parameters:
    ///   - row: The row the oscillator is in.
    ///   - column: The column the oscillator is in.
    ///   - detune: The amount of cents to detune this oscillator.
    void changeDetuneCents(int row, int column, float detune);
    
    // MASTER ADSR
    
    
    /// Changes the gain of this  voice.
    /// - Parameter gain: The gain val in decibels.
    void changeGain(float gain);
    
    
    /// Changes the attack of the ADSR for this voice.
    /// - Parameter attackVal: The attack value.
    void changeAttack(float attackVal);
    
    
    /// Changes the release for this voice.
    /// - Parameter releaseVal: The release value.
    void changeRelease(float releaseVal);
    
    
    /// Changes the decay for this voice.
    /// - Parameter decayVal: The decay value.
    void changeDecay(float decayVal);
    
    
    /// Changes the sustain for this voice.
    /// - Parameter sustainVal: The sustain value.
    void changeSustain(float sustainVal);
    
    // ROW ADSR
    
    
    /// Changes the gain for one of the rows of oscillators.
    /// - Parameters:
    ///   - row: The row to edit.
    ///   - gain: The new gain value.
    void changeRowGain(float row, float gain);
    
    
    /// Changes the attack for one of the rows of oscillators.
    /// - Parameters:
    ///   - row: The row of oscillators to edit.
    ///   - attack: The new attack value.
    void changeRowAttack(float row, float attack);
    
    
    /// Changes the release for one of the rows of oscillators.
    /// - Parameters:
    ///   - row: The row of oscillators to edit.
    ///   - release: The new release value.
    void changeRowRelease(float row, float release);
    
    
    /// Changes the decay for one of the rows of oscillators.
    /// - Parameters:
    ///   - row: The row of oscillators to edit.
    ///   - decay: The new decay value.
    void changeRowDecay(float row, float decay);
    
    
    /// Changes the sustain of one of the rows of oscillators.
    /// - Parameters:
    ///   - row: The row to edit.
    ///   - sustain: The new sustain value.
    void changeRowSustain(float row, float sustain);
    
    // FOR THE FILTER
    
    
    /// Changes the filter cutoff of one of the rows of oscillators.
    /// - Parameters:
    ///   - row: The row to edit.
    ///   - newFrequency: The new cutoff value.
    void changeFilterCutoff(int row, float newFrequency);
    
    
    /// Changes the filter resonance of one of the rows of oscillators.
    /// - Parameters:
    ///   - row: The row to edit.
    ///   - resonance: The new resonance value.
    void changeFilterResonance(int row, float resonance);
    
    
    /// Changes the filter type for one of the rows of oscillators.
    /// - Parameters:
    ///   - row: The row to edit.
    ///   - type: The new type of filter (every filter has representative number).
    void changeFilterType(int row, int type);
    
    // FOR THE LFO
    
    
    /// Changes the frequency of the LFO for all oscillators.
    /// - Parameter rate: The new frequency of the LFO in hz.
    void changeLfoFrequency(float rate);
    
    
    /// Changes the depth of the LFO for all oscillators.
    /// - Parameter depth: The new depth of the LFO.
    void changeLfoDepth(float depth);
    
    
    /// Changes the wave type for the LFO for all oscillators.
    /// - Parameter index: The index of the wave type.
    void changeLfoWaveType(float index);
    
    // FOR THE CUSTOM OSCILLATOR
    
    
    /// Changes the wave type of the custom oscillator.
    /// - Parameters:
    ///   - typeIndex: The type of wave 0 - COSINE, 1- SINE
    ///   - accuracy: How many times the series repeats
    ///   - mulVal: How many times the series is multipled.
    ///   - repeatX: The power of X.
    ///   - repeatN: The power of N (aka the repeat value).
    void changeCustomOscFunction(int typeIndex, int accuracy, float mulVal, float repeatX, float repeatN);
    
    
    /// Changes if the custom oscillator is active.
    /// - Parameter isActive: true - it is active, false - it is not active.
    void changeCustomOscIsActive(bool isActive);
    
    
    /// Changes the pitch of the custom oscillator.
    /// - Parameter pitch: The new pitch.
    void changeCustomOscPitch(float pitch);
    
    // FOR THE REVERB:
    
    /// Changes the room size of the reverb.
    /// - Parameter roomSize: The new room size (0 - 1).
    void changeReverbRoomSize(float roomSize);
    
    
    /// Changes the damping of the reverb.
    /// - Parameter damping: The damping value (0 - 1).
    void changeReverbDamping(float damping);
    
    
    /// Changes the wet level of the reverb.
    /// - Parameter wetLevel: The new wet level value (0 - 1).
    void changeReverbWetLevel(float wetLevel);
    
    
    /// Changes the dry level of the reverb.
    /// - Parameter dryLevel: The new dry level value (0 - 1).
    void changeReverbDryLevel(float dryLevel);
    
    
    /// Changes the width of the reverb.
    /// - Parameter width: The new width value (0 - 1).
    void changeReverbWidth(float width);
    
    
    /// The new freeze level of the reverb.
    /// - Parameter freezeMode: The new freeze level (0 - 0.5).
    void changeReverbFreezeMode(float freezeMode);
    
    
    /// Sets the reverb as active or not.
    /// - Parameter isActive: true - the reverb is active, false - the reverb is not active.
    void setReverbActive(bool isActive);
    
    // FOR THE CHORUS:
    
    /// Sets the rate of the chorus.
    /// - Parameter rate: The new rate of the chorus (0 - 10).
    void changeChorusRate(float rate);
    
    
    /// Sets the depth of the chorus.
    /// - Parameter depth: The new depth value (0 - 1).
    void changeChorusDepth(float depth);
    
    
    /// Sets the centre delay of the chorus.
    /// - Parameter delay: The delay value (0 - 1000).
    void changeChorusCentreDelay(float delay);
    
    
    /// Sets the feedback of the chorus.
    /// - Parameter feedback: The feedback value of the chorus (-1 - 1).
    void changeChorusFeedback(float feedback);
    
    
    /// Sets the mix of the chorus.
    /// - Parameter mix: The mix value (0 - 1).
    void changeChorusMix(float mix);
    
    
    /// Sets the chorus as active or not.
    /// - Parameter isActive: true - chorus is active, false - chorus is inactive.
    void changeChorusActive(bool isActive);
    
private:
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;
    std::vector<OscillatorRow*> oscillatorRows;
    juce::dsp::Gain<float> gain;
    
    CustomOscillator customOsc;
    juce::dsp::Reverb masterReverb;
    juce::dsp::Reverb::Parameters reverbParams;
    
    juce::dsp::Chorus<float> chorus;
    bool chorusActive;
    
    
    /// Updates the master ADSR values.
    void updateADSR();
    
};

