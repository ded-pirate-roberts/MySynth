/*
  ==============================================================================

    OscillatorRow.cpp
    Created: 29 Jun 2025 8:50:24pm
    Author:  Caleb Balboni

  ==============================================================================
*/

#include "OscillatorRow.h"

/*
 * CONSTRUCTOR FOR THE ROW OF OSCILLATORS, ADDS THREE OSCILLATORS TO THIS PARTICULAR ROW
 */
OscillatorRow::OscillatorRow() {
    for (int i = 0; i < 3; i++) {
        oscillators.push_back(std::unique_ptr<Oscillator>(new Oscillator()));
    }
    filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    filterActive = false;
    filterResonance = 0.001f;
    filterCutoff = 0.0f;
    lfo = std::unique_ptr<Oscillator>(new Oscillator());
    lfo->setActive(true);
    lfoDepth = 0.5f;
    lfoFrequency = 4.0f;
    baseFrequency = 0.0f;
    
    adsrParams.attack = 0.0f;
    adsrParams.sustain = 0.1f;
    adsrParams.release = 0.1f;
    adsrParams.decay = 0.1f;
    
    gain.setGainDecibels(1.0f);
}


void OscillatorRow::getNextAudioBlock (juce::dsp::AudioBlock<float>& dest)
{
    juce::AudioBuffer<float> rowBuf ((int)dest.getNumChannels(),
                                     (int)dest.getNumSamples());
    adsr.applyEnvelopeToBuffer(rowBuf, 0.0f, dest.getNumSamples());
    rowBuf.clear();
    juce::dsp::AudioBlock<float> rowBlock (rowBuf);
    juce::AudioBuffer<float> scratchBuf (rowBuf.getNumChannels(),
                                         rowBuf.getNumSamples());
    juce::dsp::AudioBlock<float> scratchBlock (scratchBuf);
    juce::AudioBuffer<float> lfoBuf (1, dest.getNumSamples());
    juce::dsp::AudioBlock<float>  lfoBlock (lfoBuf);
    lfo->process (juce::dsp::ProcessContextReplacing<float> (lfoBlock));
    for (std::unique_ptr<Oscillator>& osc : oscillators)
    {
        if (! osc->isActive())
            continue;

        scratchBuf.clear();
        for (int i = 0; i < dest.getNumSamples(); ++i)
        {
            float lfoVal = lfoBuf.getSample (0, i);
            float modHz  = baseFrequency * (1.0f + lfoVal * lfoDepth);
            osc->changeFrequency (modHz);
            float out = osc->processSample (0.0f) * 0.2f;
            for (int ch = 0; ch < scratchBuf.getNumChannels(); ++ch)
                   scratchBuf.setSample (ch, i, out);
        }
        rowBlock.add (scratchBlock);
    }
    updateFilter();
    gain.process(juce::dsp::ProcessContextReplacing<float>(rowBlock));
    if (filterActive)
        filter.process (juce::dsp::ProcessContextReplacing<float>(rowBlock));
    dest.add (rowBlock);
}

void OscillatorRow::updateFilter() {
    float modulator = adsr.getNextSample() * filterCutoff;
    modulator = std::min(20000.0f, modulator);
    modulator = std::max(20.0f, modulator);
    filter.setCutoffFrequency  (modulator);
    filter.setResonance        (filterResonance);
}

void OscillatorRow::updateADSR() {
    adsr.setParameters(adsrParams);
}

void OscillatorRow::changeGainDecibels(float decibels) {
    gain.setGainDecibels(decibels);
    updateADSR();
}

void OscillatorRow::changeAdsrAttack(float attack) {
    adsrParams.attack = attack;
    updateADSR();
}

void OscillatorRow::changeAdsrDecay(float decay) {
    adsrParams.decay = decay;
    updateADSR();
}

void OscillatorRow::changeAdsrSustain(float sustain) {
    adsrParams.sustain = sustain;
    updateADSR();
}

void OscillatorRow::changeAdsrRelease(float release) {
    adsrParams.release = release;
    updateADSR();
}


void OscillatorRow::startNote(int midiNote) {
    baseFrequency = juce::MidiMessage::getMidiNoteInHertz(midiNote);
    for (std::unique_ptr<Oscillator>& osc : oscillators) {
        if (osc->isActive()) {
            osc->reset();
            osc->changeFrequency(baseFrequency);
        }
    }
    lfo->reset();
    lfo->changeFrequency (lfoFrequency);
    adsr.reset();
    adsr.noteOn();
}

void OscillatorRow::noteOff() {
    adsr.noteOff();
}


void OscillatorRow::changeLfoDepth(float depth) {
    lfoDepth = depth;
}


void OscillatorRow::changeLfoWaveType(float index) {
    lfo->changeWaveType(index);
}


void OscillatorRow::changeLfoFrequency(float frequency) {
    lfoFrequency = frequency;
}


void OscillatorRow::prepareRow(juce::dsp::ProcessSpec spec) {
    for (std::unique_ptr<Oscillator>& osc : oscillators) {
        osc->prepare(spec);
    }
    if (lfo->isActive()) {
        lfo->prepare   (spec);
    }
    adsr.setSampleRate(spec.sampleRate);
    gain.prepare(spec);
    filter.prepare (spec);
    filter.reset();
}


void OscillatorRow::changeDetuneCents(int x, float cents) {
    oscillators[x - 1]->setDetuneCents(cents);
}


void OscillatorRow::changeFilter(int index) {
    if (index == 0) {
        filterActive = false;
        return;
    }
    if (index == 1) {
        filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
        filterActive = true;
        return;
    }
    if (index == 2) {
        filter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
        filterActive = true;
        return;
    }
    if (index == 3) {
        filter.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
        filterActive = true;
        return;
    }
    
}


void OscillatorRow::changeFilterCutoff(float newFrequency) {
    this->filterCutoff = newFrequency;
}


void OscillatorRow::changeFilterResonance(float resonance) {
    this->filterResonance = resonance;
}

void OscillatorRow::changeOscillatorIsActive(int index, bool isActive) {
    oscillators[index - 1]->setActive(isActive);
}

void OscillatorRow::changeOscillatorWaveForm(int index, int waveForm) {
    oscillators[index - 1]->changeWaveType(waveForm);
}

