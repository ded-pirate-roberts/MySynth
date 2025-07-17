/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthVoice.h"
#include <vector>

//==============================================================================


/// Main controller class for the program. Controls taking in midi inputs from the DAW and updating params from the GUI.
class MySynthAudioProcessor  : public juce::Component,
                               public juce::AudioProcessor
{
public:
    
    //==============================================================================
    MySynthAudioProcessor();
    ~MySynthAudioProcessor() override;
                                   
    juce::AudioProcessorValueTreeState apvts;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif
    
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    
private:
    
    
    /// Sets up the parameters for the APVTS.
    juce::AudioProcessorValueTreeState::ParameterLayout setupParams();
    
    
    /// Updates the wave types for all the oscillators.
    /// - Parameter voice: The voice of whose oscillators will be edited.
    void updateWaveType(SynthVoice* voice);
    
    
    /// Updates the pitch for all oscillators.
    /// - Parameter voice: The voice of whose oscillators pitch will be updated.
    void updateOscPitch(SynthVoice* voice);
    
    
    /// Updates all oscillators and sets them active or not.
    /// - Parameter voice: The voice whose oscillators are to be updated.
    void updateOscActive(SynthVoice* voice);
    
    
    /// Updates the ADSR and Gain values for all oscillators.
    /// - Parameter voice: The voice of whose oscillators are to be updated.
    void updateAdsrGain(SynthVoice* voice);
    
    
    /// Updates the filters for all oscillator rows.
    /// - Parameter voice: The voice of whose rows of oscillators are to be edited.
    void updateFilter(SynthVoice* voice);
    
    
    /// Updates the LFO for all oscillators.
    /// - Parameter voice: The voice of whose LFO is to be edited.
    void updateLfo(SynthVoice* voice);
    
    
    /// Updates the parameters for the custom oscillator.
    /// - Parameter voice: The voice of whose custom oscillators is to be edited.
    void updateCustomOsc(SynthVoice* voice);
    
    
    /// Updates the master reverb settings.
    /// - Parameter voice: The voice of whose reverb is to be updated.
    void updateReverbSetting(SynthVoice* voice);
    
    
    /// Updates the master chorus settings.
    /// - Parameter voice: The voice of whose chorus is to be updated.
    void updateChorusSetting(SynthVoice* voice);
    
    juce::dsp::WaveShaper<float> softClip;
    juce::dsp::Gain<float> masterGain;
    juce::Synthesiser synthesiser;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MySynthAudioProcessor)
};

