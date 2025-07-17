
#pragma once
#include <JuceHeader.h>


/// Synth sound object.
class SynthSound : public juce::SynthesiserSound {
public:
    bool appliesToChannel(int midiChannel) { return true; }
    bool appliesToNote(int note) { return true; }
};

