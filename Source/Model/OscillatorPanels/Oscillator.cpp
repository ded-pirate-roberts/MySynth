/*
  ==============================================================================

    Oscillator.cpp
    Created: 25 Jun 2025 5:26:37pm
    Author:  Caleb Balboni

  ==============================================================================
*/

#include "Oscillator.h"

Oscillator::Oscillator() {
    initialise([] (float x) { return std::sin(x);});
    active = false;
    detuneCents = 0.0f;
    fmMod = 0.0f;
}

bool Oscillator::isActive() {
    return active;
}

void Oscillator::setActive(bool isActive) {
    active = isActive;
}

double Oscillator::sigmoid(float x) {
    return 1.0 / (1.0 + std::exp(-x));
}

float Oscillator::saw (float x)                // phase in range 0…1
{
    return (1.0f/juce::MathConstants<float>::pi) * (x - juce::MathConstants<float>::pi);
}

float Oscillator::tri (float x)               // phase ∈ [0,1)
{
    return (2.0f/juce::MathConstants<float>::pi) * asinf(std::sin(x));
}

float Oscillator::doubleSine(float x) {
    return std::sin(x) + (std::sin(2 * x) / 2.0f) + (std::sin(3 * x) / 3.0f);
}

float Oscillator::doubleCos(float x) {
    return std::cos(x) + (std::cos(std::pow(x, 2.0f) * 4.0f) / 2.0f) + (std::cos(std::pow(x, 3.0f) * 8.0f) / 3.0f);
}

float Oscillator::whiteNoise(float x) {
    return juce::Random::getSystemRandom().nextFloat() * 0.25f - 0.125f;
}

float Oscillator::powerSine(float x) {
    float s = std::sin(x);
    float n = 3.0f;
    return (s >= 0.0f) ? std::pow(s, n) : -std::pow(-s, n);
}

float Oscillator::sincWave(float x) {
    if (std::abs(x) < 1e-6f) {
        return 1.0f;
    }
    return std::sin(x) / x;
}

float Oscillator::polyWave(float x) {
    float y = x / juce::MathConstants<float>::pi;
    return (4.0f/3.0f) * (y - (y*y*y)/3.0f);
}

float Oscillator::hyperbolicTan(float x) {
    float s = std::sin(x);
    return std::tanh(2.0f * s);
}

float Oscillator::softSquare(float x) {
    return (2.0f/juce::MathConstants<float>::pi) * std::atan(std::tan(x * 0.5f) * 5.0f);
}

float Oscillator::halfSine(float x) {
    float s = std::sin (x);
    return 2.0f * std::max (0.0f, s) - 1.0f;
}

void Oscillator::setDetuneCents(float detune) {
    this->detuneCents = detune;
}

void Oscillator::changeFrequency (float frequency)
{
    float ratio    = std::pow (2.0f, detuneCents / 1200.0f);
    float hz       = frequency * ratio + fmMod;          // ← remember it
    setFrequency (hz);              // JUCE call
}

void Oscillator::changeWaveType(int type) {
    if (type == 0) {
        initialise([](float x) { return std::sin(x); }, 128);
        return;
    }
    if (type == 1) {
        initialise([](float x) {
            return x < 0.0f ? 1.0f : -1.0f;
        });
        return;
    }
    if (type == 2) {
        initialise([this] (float x) { return sigmoid(x); }, 128);
        return;
    }
    
    if (type == 3) {
        initialise([this] (float x) {return saw(x); }, 128);
        return;
    }
    if (type == 4) {
        initialise([this] (float x) {return saw(x); }, 8);
        return;
    }
    if (type == 5) {
        initialise([this] (float x) {return tri(x); }, 128);
        return;
    }
    if (type == 6) {
        initialise([this] (float x) {return halfSine(x); }, 128);
        return;
    }
    if (type == 7) {
        initialise([this] (float x) {return doubleSine(x); }, 128);
        return;
    }
    if (type == 8) {
        initialise([this] (float x) {return doubleCos(x); }, 128);
        return;
    }
    if (type == 9) {
        initialise([this] (float x) {return whiteNoise(x); }, 128);
        return;
    }
    if (type == 10) {
        initialise([this] (float x) {return powerSine(x); }, 128);
        return;
    }
    if (type == 11) {
        initialise([this] (float x) {return sincWave(x); }, 128);
        return;
    }
    if (type == 12) {
        initialise([this] (float x) {return softSquare(x); }, 128);
        return;
    }
    if (type == 13) {
        initialise([this] (float x) {return polyWave(x); }, 128);
        return;
    }
    if (type == 15) {
        initialise([this] (float x) {return hyperbolicTan(x); }, 128);
        return;
    }
}


void Oscillator::getNextAudioBlock (juce::dsp::AudioBlock<float>& block)
{
    processFmOsc (block);
    process (juce::dsp::ProcessContextReplacing<float> (block));
    //block.multiplyBy(0.2f);
}

void Oscillator::processFmOsc (juce::dsp::AudioBlock<float>& block)
{
    for (int ch = 0; ch < block.getNumChannels(); ++ch)
    {
        for (int s = 0; s < block.getNumSamples(); ++s)
        {
          //  fmMod = fmOsc.processSample (block.getSample (ch, s)) * fmDepth;
        }
    }
}

void Oscillator::updateFm (const float freq, const float depth)
{
   // fmOsc.setFrequency (freq);
    fmDepth = depth;
   // auto currentFreq = juce::MidiMessage::getMidiNoteInHertz (lastMidiNote) + fmMod;
   // setFrequency (currentFreq >= 0 ? currentFreq : currentFreq * -1.0f);
}

