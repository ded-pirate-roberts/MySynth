/*
  ==============================================================================

    CustomOscillator.cpp
    Created: 1 Jul 2025 6:02:24pm
    Author:  Caleb Balboni

  ==============================================================================
*/

#include "CustomOscillator.h"

CustomOscillator::CustomOscillator() {
    pitchVal = 0.0f;
    active = false;
    typeIndex = 0;
    accuracy = 1;
    mulVal = 1;
    repeatX = 1;
    repeatN = 1;
    
}

float CustomOscillator::customFunction(float x) {
    float retVal = 0.0f;
    if (typeIndex == 0) {
        for (int i = 1.0f; i <= accuracy; i++) {
            retVal += std::cos(mulVal * std::pow(x, repeatX) * std::pow(i, repeatN));
        }
    } else if (typeIndex == 1) {
        for (int i = 1.0f; i <= accuracy; i++) {
            retVal += std::sin(mulVal * std::pow(x, repeatX) * std::pow(i, repeatN));
        }
    }
    return std::clamp(retVal, -10.0f, 10.0f);
}

void CustomOscillator::changeIsActive(bool isActive) {
    active = isActive;
}

void CustomOscillator::changePitch(float pitchVal) {
    this->pitchVal = pitchVal;
}

bool CustomOscillator::isActive() {
    return active;
}

void CustomOscillator::getNextAudioBlock (juce::dsp::AudioBlock<float>& block)
{
    process (juce::dsp::ProcessContextReplacing<float> (block));
    block.multiplyBy(0.2f);
}

void CustomOscillator::changeFrequency (float frequency)
{
    float ratio    = std::pow (2.0f, pitchVal / 1200.0f);
    float hz = frequency * ratio;
    setFrequency (hz);           
}

void CustomOscillator::changeFunctionType(int typeIndex, int accuracy, float mulVal, float repeatX, float repeatN) {
    this->typeIndex = typeIndex;
    this->accuracy = accuracy;
    this->mulVal = mulVal;
    this->repeatX = repeatX;
    this->repeatN = repeatN;
    initialise([this](float x) { return customFunction(x); }, 128);
}
