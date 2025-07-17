/*
  ==============================================================================

    SynthVoice.cpp
    Created: 22 Jun 2025 2:26:16pm
    Author:  caleb

  ==============================================================================
*/

#include "SynthVoice.h"
using namespace std;

SynthVoice::SynthVoice() {
    for (int i = 0; i < 3; i++) {
        oscillatorRows.push_back(new OscillatorRow());
    }
    // ADSR
    adsrParams.attack = 0.0f;
    adsrParams.sustain = 0.1f;
    adsrParams.release = 0.1f;
    adsrParams.decay = 0.1f;
    gain.setGainDecibels(1.0f);
    
    // SET UP FOR REVERB BS
    masterReverb.setEnabled(true);
    reverbParams.width = 0.5f;
    reverbParams.damping = 0.5f;
    reverbParams.dryLevel = 0.5f;
    reverbParams.freezeMode = 0.5f;
    reverbParams.wetLevel = 0.5f;
    masterReverb.setParameters(reverbParams);
    chorus.setRate(0.0f);
    chorus.setDepth(0.0f);
    chorus.setCentreDelay(0.0f);
    chorus.setFeedback(0.0f);
    chorus.setMix(0.0f);
    chorusActive = false;
}

SynthVoice::~SynthVoice() {
    for (OscillatorRow* row : oscillatorRows) {
        delete row;
    }
    oscillatorRows.clear();
}

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound) {

    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::stopNote(float velo, bool allowTailOff) {
    adsr.noteOff();
}

void SynthVoice::startNote(int midiNoteNum, float velo, juce::SynthesiserSound* sound, int currentPitchWheelPos) {
    
    adsr.reset();
    for (OscillatorRow* oscs : oscillatorRows) {
        oscs->startNote(midiNoteNum);
    }
    if (customOsc.isActive()) {
        customOsc.changeFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNum));
    }
    customOsc.reset();
    chorus.reset();
    masterReverb.reset();
    masterReverb.setParameters(reverbParams);
    updateADSR();
    adsr.noteOn();
}

void SynthVoice::changeLfoFrequency(float rate) {
    for (OscillatorRow* osc : oscillatorRows) {
        osc->changeLfoFrequency(rate);
    }
}

void SynthVoice::changeLfoDepth(float depth) {
    for (OscillatorRow* osc : oscillatorRows) {
        osc->changeLfoDepth(depth);
    }
}

void SynthVoice::changeLfoWaveType(float index) {
    for (OscillatorRow* osc : oscillatorRows) {
        osc->changeLfoWaveType(index);
    }
}

void SynthVoice::renderNextBlock (juce::AudioBuffer<float>& outputBuffer,
                                  int startSample, int numSamples)
{
    if (!isVoiceActive())
        return;

    juce::AudioBuffer<float> voiceBuf (outputBuffer.getNumChannels(), numSamples);
    voiceBuf.clear();
    juce::dsp::AudioBlock<float> voiceBlock (voiceBuf);
    juce::AudioBuffer<float> scratch (voiceBuf.getNumChannels(), numSamples);
    juce::dsp::AudioBlock<float> scratchBlock (scratch);

    for (OscillatorRow* osc : oscillatorRows)
    {
        scratch.clear();
        osc->getNextAudioBlock (scratchBlock);
        voiceBlock.add (scratchBlock);
    }

    if (customOsc.isActive()) {
        scratch.clear();
        customOsc.getNextAudioBlock(scratchBlock);
        voiceBlock.add (scratchBlock);
    }
    adsr.applyEnvelopeToBuffer (voiceBuf, 0, numSamples);
    gain.process (juce::dsp::ProcessContextReplacing<float> (voiceBlock));
    
    if (masterReverb.isEnabled()) {
        masterReverb.process(juce::dsp::ProcessContextReplacing<float>(voiceBlock));
    }
    if (chorusActive) {
        chorus.process(juce::dsp::ProcessContextReplacing<float>(voiceBlock));
    }
    
    for (int ch = 0; ch < outputBuffer.getNumChannels(); ++ch)
        outputBuffer.addFrom (ch, startSample, voiceBuf, ch, 0, numSamples);

    if (!adsr.isActive())
        clearCurrentNote();
}

void SynthVoice::changeCustomOscFunction(int typeIndex, int accuracy, float mulVal, float repeatX, float repeatN) {
    customOsc.changeFunctionType(typeIndex, accuracy, mulVal, repeatX, repeatN);
}

void SynthVoice::changeCustomOscIsActive(bool isActive) {
    customOsc.changeIsActive(isActive);
}

void SynthVoice::changeCustomOscPitch(float pitch) {
    customOsc.changePitch(pitch);
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue) {

}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue) {

}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels) {
    adsr.setSampleRate(sampleRate);
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
    gain.prepare(spec);
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
    masterReverb.prepare(spec);
    chorus.prepare(spec);
    for (OscillatorRow* osc : oscillatorRows) {
        osc->prepareRow(spec);
    }
    
    if (customOsc.isActive()) {
        customOsc.prepare(spec);
    }
}

void SynthVoice::changeGain(float value) {
    gain.setGainDecibels(value);
}


void SynthVoice::changeOscillatorWaveType(int row, int column, int type) {
    oscillatorRows[row - 1]->changeOscillatorWaveForm(column, type);
}

void SynthVoice::changeOscillatorIsActive(int row, int column, bool isActive) {
    oscillatorRows[row - 1]->changeOscillatorIsActive(column, isActive);
}

void SynthVoice::changeDetuneCents(int row, int column, float detune) {
    oscillatorRows[row - 1]->changeDetuneCents(column, detune);
}

// MASTER ADSR
void SynthVoice::updateADSR() {
    adsr.setParameters(adsrParams);
}

void SynthVoice::changeAttack(float attackVal) {
    adsrParams.attack = attackVal;
    updateADSR();
}

void SynthVoice::changeRelease(float releaseVal) {
    adsrParams.release = releaseVal;
    updateADSR();
}

void SynthVoice::changeDecay(float decayVal) {
    adsrParams.decay = decayVal;
    updateADSR();
}

void SynthVoice::changeSustain(float sustainVal) {
    adsrParams.sustain = sustainVal;
    updateADSR();
}

// ROW ADSR

void SynthVoice::changeRowGain(float x, float gain) {
    oscillatorRows[x - 1]->changeGainDecibels(gain);
}

void SynthVoice::changeRowAttack(float x, float attack) {
    oscillatorRows[x - 1]->changeAdsrAttack(attack);
}

void SynthVoice::changeRowRelease(float x, float release) {
    oscillatorRows[x - 1]->changeAdsrRelease(release);
}

void SynthVoice::changeRowDecay(float x, float decay) {
    oscillatorRows[x - 1]->changeAdsrDecay(decay);
}

void SynthVoice::changeRowSustain(float x, float sustain) {
    oscillatorRows[x - 1]->changeAdsrSustain(sustain);
}

void SynthVoice::changeFilterCutoff(int y, float newFrequency) {
    oscillatorRows[y - 1]->changeFilterCutoff(newFrequency);
}

void SynthVoice::changeFilterResonance(int y, float resonance) {
    oscillatorRows[y - 1]->changeFilterResonance(resonance);
}

void SynthVoice::changeFilterType(int y, int type) {
    oscillatorRows[y - 1]->changeFilter(type);
}


// SETTERS FOR THE REVERB

void SynthVoice::changeReverbRoomSize(float roomSize) {
    reverbParams.roomSize = roomSize;
}

void SynthVoice::changeReverbDamping(float damping) {
    reverbParams.damping = damping;
}

void SynthVoice::changeReverbWetLevel(float wetLevel) {
    reverbParams.wetLevel = wetLevel;
}

void SynthVoice::setReverbActive(bool isActive) {
    masterReverb.setEnabled(isActive);
}

void SynthVoice::changeReverbDryLevel(float dryLevel) {
    reverbParams.dryLevel = dryLevel;
}

void SynthVoice::changeReverbWidth(float width) {
    reverbParams.width = width;
}

void SynthVoice::changeReverbFreezeMode(float freezeMode) {
    reverbParams.freezeMode = freezeMode;
}

// FOR CHORUS

void SynthVoice::changeChorusRate(float rate) {
    chorus.setRate(rate);
}

void SynthVoice::changeChorusDepth(float depth) {
    chorus.setDepth(depth);
}

void SynthVoice::changeChorusCentreDelay(float delay) {
    chorus.setCentreDelay(delay);
}

void SynthVoice::changeChorusFeedback(float feedback) {
    chorus.setFeedback(feedback);
}

void SynthVoice::changeChorusMix(float mix) {
    chorus.setMix(mix);
}

void SynthVoice::changeChorusActive(bool isActive) {
    chorusActive = isActive;
}

