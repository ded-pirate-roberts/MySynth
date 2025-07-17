/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "SynthSound.h"
#include "SynthVoice.h"
#include <vector>
using namespace std;
//==============================================================================
MySynthAudioProcessor::MySynthAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", setupParams())
{
    synthesiser.addSound(new SynthSound());
    for (int i = 0; i < 32; i++) {
        synthesiser.addVoice(new SynthVoice());
    }

    synthesiser.setNoteStealingEnabled (true);
    setLatencySamples (0);
    setBusesLayout (getBusesLayout());
}

MySynthAudioProcessor::~MySynthAudioProcessor() {

}

//==============================================================================
const juce::String MySynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MySynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MySynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MySynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MySynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MySynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MySynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MySynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MySynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void MySynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MySynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synthesiser.setCurrentPlaybackSampleRate(sampleRate);
    juce::dsp::ProcessSpec spec { sampleRate,
                                   (juce::uint32) samplesPerBlock,
                                   (juce::uint32) getTotalNumOutputChannels() };

     softClip.functionToUse = [] (float x)
     {
         return juce::jlimit (-1.0f, 1.0f, 0.5f * (x + std::tanh (x)));
     };
    
    
    masterGain.prepare (spec);
    masterGain.setGainDecibels(-6.0f);
    softClip.prepare (spec);
    for (int i = 0; i < synthesiser.getNumVoices(); i++) {
        if (auto* voice = dynamic_cast<SynthVoice*>(synthesiser.getVoice(i))) {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
}

void MySynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MySynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MySynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

  
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int i = 0; i < synthesiser.getNumVoices(); i++) {
        if (auto* voice = dynamic_cast<SynthVoice*>(synthesiser.getVoice(i))) {
            updateWaveType(voice);
            updateOscActive(voice);
            updateAdsrGain(voice);
            updateOscPitch(voice);
            updateFilter(voice);
            updateLfo(voice);
            updateCustomOsc(voice);
            updateReverbSetting(voice);
            updateChorusSetting(voice);
        }
    }
    synthesiser.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    juce::dsp::AudioBlock<float> audioBlock{ buffer };
    juce::dsp::ProcessContextReplacing<float> ctx (audioBlock);
    masterGain.process(ctx);
    softClip.process (ctx);
}

void MySynthAudioProcessor::updateWaveType(SynthVoice* voice) {
    for (int x = 1; x <= 3; x++) {
        for (int y = 1; y <= 3; y++) {
            std::atomic<float>* waveType = apvts.getRawParameterValue("OSC" + std::to_string(x) + std::to_string(y) + "WAVETYPE");
            int waveIndex = static_cast<int> (waveType->load());
            voice->changeOscillatorWaveType(x,y, waveIndex);
        }
    }
}

void MySynthAudioProcessor::updateOscActive(SynthVoice* voice) {
    for (int x = 1; x <= 3; x++) {
        for (int y = 1; y <= 3; y++) {
            juce::String paramID = "OSC" + juce::String (x) + juce::String (y) + "ACTIVE";
            bool isActive = static_cast<bool> (apvts.getParameterAsValue (paramID).getValue());
            voice->changeOscillatorIsActive (x,y, isActive);
        }
    }
}

void MySynthAudioProcessor::updateAdsrGain(SynthVoice* voice) {
    std::atomic<float>* gainVal = apvts.getRawParameterValue("MASTERGAIN");
    float gain = static_cast<float>(gainVal->load());
    voice->changeGain(gain);
    
    std::atomic<float>* attackVal = apvts.getRawParameterValue("MASTERATTACK");
    float attack = static_cast<float>(attackVal->load());
    voice->changeAttack(attack);
    
    std::atomic<float>* decayVal = apvts.getRawParameterValue("MASTERDECAY");
    float decay = static_cast<float>(decayVal->load());
    voice->changeDecay(decay);
    
    std::atomic<float>* sustainVal = apvts.getRawParameterValue("MASTERSUSTAIN");
    float sustain = static_cast<float>(sustainVal->load());
    voice->changeSustain(sustain);
    
    std::atomic<float>* releaseVal = apvts.getRawParameterValue("MASTERRELEASE");
    float release = static_cast<float>(releaseVal->load());
    voice->changeRelease(release);
    
}

void MySynthAudioProcessor::updateOscPitch(SynthVoice* voice) {
    for (int x = 1; x <= 3; x++) {
        for (int y = 1; y <= 3; y++) {
            std::atomic<float>* pitchVal = apvts.getRawParameterValue("OSC" + std::to_string(x) + std::to_string(y) + "PITCH");
            int pitch = static_cast<int> (pitchVal->load());
            voice->changeDetuneCents(x,y, pitch);
        }
    }
}

void MySynthAudioProcessor::updateFilter(SynthVoice* voice) {
    for (int x = 1; x <= 3; x++) {
        std::atomic<float>* filterType = apvts.getRawParameterValue("OSC" + std::to_string(x) + "FILTERTYPE");
        int filter = static_cast<int> (filterType->load());
        voice->changeFilterType(x, filter);
            
        std::atomic<float>* cutoffVal = apvts.getRawParameterValue("OSC" + std::to_string(x) + "CUTOFF");
        float cutoff = static_cast<float> (cutoffVal->load());
        voice->changeFilterCutoff(x, cutoff);
            
        std::atomic<float>* resonanceVal = apvts.getRawParameterValue("OSC" + std::to_string(x) + "RESONANCE");
        float resonance = static_cast<float> (resonanceVal->load());
        voice->changeFilterResonance(x, resonance);
        
        std::atomic<float>* gainVal = apvts.getRawParameterValue("OSC" + std::to_string(x) + "GAIN");
        float gain = static_cast<float>(gainVal->load());
        voice->changeRowGain(x, gain);
        
        std::atomic<float>* attackVal = apvts.getRawParameterValue("OSC" + std::to_string(x) + "ATTACK");
        float attack = static_cast<float>(attackVal->load());
        voice->changeRowAttack(x, attack);
        
        std::atomic<float>* decayVal = apvts.getRawParameterValue("OSC" + std::to_string(x) + "DECAY");
        float decay = static_cast<float>(decayVal->load());
        voice->changeRowDecay(x, decay);
        
        std::atomic<float>* sustainVal = apvts.getRawParameterValue("OSC" + std::to_string(x) + "SUSTAIN");
        float sustain = static_cast<float>(sustainVal->load());
        voice->changeRowSustain(x, sustain);
        
        std::atomic<float>* releaseVal = apvts.getRawParameterValue("OSC" + std::to_string(x) + "RELEASE");
        float release = static_cast<float>(releaseVal->load());
        voice->changeRowRelease(x, release);
    }
}

void MySynthAudioProcessor::updateLfo(SynthVoice* voice) {
    std::atomic<float>* waveType = apvts.getRawParameterValue("LFO1WAVETYPE");
    int waveIndex = static_cast<int> (waveType->load());
    voice->changeLfoWaveType(waveIndex);
    
    std::atomic<float>* frequencyVal = apvts.getRawParameterValue("LFO1FREQUENCY");
    float frequency = static_cast<float> (frequencyVal->load());
    voice->changeLfoFrequency(frequency);
    
    std::atomic<float>* depthVal = apvts.getRawParameterValue("LFO1DEPTH");
    float depth = static_cast<float> (depthVal->load());
    voice->changeLfoDepth(depth);
}

void MySynthAudioProcessor::updateCustomOsc(SynthVoice* voice) {
    std::atomic<float>* functionType = apvts.getRawParameterValue("CUSTOMOSCFUNCTIONTYPE");
    int function = static_cast<int> (functionType->load());
    
    std::atomic<float>* accuracyVal = apvts.getRawParameterValue("CUSTOMOSCACCURACY");
    float accuracy = static_cast<float> (accuracyVal->load());
    
    std::atomic<float>* mutliplyVal = apvts.getRawParameterValue("CUSTOMOSCMUL");
    float multiply = static_cast<float> (mutliplyVal->load());
    
    std::atomic<float>* repeatNVal = apvts.getRawParameterValue("CUSTOMOSCREPEATN");
    float repeatN = static_cast<float> (repeatNVal->load());
    
    std::atomic<float>* repeatXVal = apvts.getRawParameterValue("CUSTOMOSCREPEATX");
    float repeatX = static_cast<float> (repeatXVal->load());
    
    std::atomic<float>* pitchVal = apvts.getRawParameterValue("CUSTOMOSCPITCH");
    float pitch = static_cast<float> (pitchVal->load());
    
    juce::String paramID = "CUSTOMOSCENABLED";
    bool isActive = static_cast<bool> (apvts.getParameterAsValue (paramID).getValue());
    
    voice->changeCustomOscFunction(function, accuracy, multiply, repeatX, repeatN);
    voice->changeCustomOscPitch(pitch);
    voice->changeCustomOscIsActive(isActive);
}

void MySynthAudioProcessor::updateReverbSetting(SynthVoice* voice) {
    std::atomic<float>* roomSizeVal = apvts.getRawParameterValue("REVERBROOMSIZE");
    float roomSize = static_cast<float> (roomSizeVal->load());
    voice->changeReverbRoomSize(roomSize);
    
    std::atomic<float>* dampingVal = apvts.getRawParameterValue("REVERBDAMPING");
    float damping = static_cast<float> (dampingVal->load());
    voice->changeReverbDamping(damping);
    
    std::atomic<float>*  wetVal = apvts.getRawParameterValue("REVERBWETLEVEL");
    float wet = static_cast<float> (wetVal->load());
    voice->changeReverbWetLevel(wet);
    
    std::atomic<float>*  dryVal = apvts.getRawParameterValue("REVERBDRYLEVEL");
    float dry = static_cast<float> (dryVal->load());
    voice->changeReverbDryLevel(dry);
    
    std::atomic<float>*  widthVal = apvts.getRawParameterValue("REVERBWIDTH");
    float width = static_cast<float> (widthVal->load());
    voice->changeReverbWidth(width);
    
    std::atomic<float>*  freezeVal = apvts.getRawParameterValue("REVERBFREEZEMODE");
    float freeze = static_cast<float> (freezeVal->load());
    voice->changeReverbFreezeMode(freeze);
    
    juce::String paramID = "REVERBACTIVE";
    bool isActive = static_cast<bool> (apvts.getParameterAsValue (paramID).getValue());
    voice->setReverbActive(isActive);
}

void MySynthAudioProcessor::updateChorusSetting(SynthVoice* voice) {
    std::atomic<float>* rateVal = apvts.getRawParameterValue("CHORUSRATE");
    float rate = static_cast<float> (rateVal->load());
    voice->changeChorusRate(rate);
    
    std::atomic<float>*  depthVal = apvts.getRawParameterValue("CHORUSDEPTH");
    float depth = static_cast<float> (depthVal->load());
    voice->changeChorusDepth(depth);
    
    std::atomic<float>*  delayVal = apvts.getRawParameterValue("CHORUSCENTREDELAY");
    float delay = static_cast<float> (delayVal->load());
    voice->changeChorusCentreDelay(delay);
    
    std::atomic<float>*  feedbackVal = apvts.getRawParameterValue("CHORUSFEEDBACK");
    float feedback = static_cast<float> (feedbackVal->load());
    voice->changeChorusFeedback(feedback);
    
    std::atomic<float>*  mixVal = apvts.getRawParameterValue("CHORUSMIX");
    float mix = static_cast<float> (mixVal->load());
    voice->changeChorusMix(mix);
    
    juce::String paramID = "CHORUSACTIVE";
    bool isActive = static_cast<bool> (apvts.getParameterAsValue (paramID).getValue());
    voice->changeChorusActive(isActive);
}

//==============================================================================
bool MySynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MySynthAudioProcessor::createEditor()
{
    return new MySynthAudioProcessorEditor (*this);
}

//==============================================================================
void MySynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
     auto state = apvts.copyState();

     std::unique_ptr<juce::XmlElement> xml (state.createXml());

     copyXmlToBinary (*xml, destData);
}

void MySynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes) {
        std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

        if (xmlState != nullptr)
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MySynthAudioProcessor();
}

// STUFF FOR DEALING WITH FIELD CHANGED TO PASS TO OUR SYNTH VOICE, AND EDITOR


juce::AudioProcessorValueTreeState::ParameterLayout MySynthAudioProcessor::setupParams() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    juce::StringArray waves = {"SINE", "SQUARE", "SIGMOID", "SAW", "BITSAW",
        "HALFSINE", "TRIANGLE", "DOUBLESINE", "DOUBLECOSINE", "WHITENOISE", "POWERSINE",
        "SINCWAVE", "SOFTSQUARE", "POLYWAVE", "HYPERTAN" };
    
    // CHOOSE THE WAVE TYPE FOR THE SPECIFIC OSCILLATOR
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC11WAVETYPE", "osc 1x1 wave type",
                                                                  waves, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC12WAVETYPE", "osc 1x2 wave type",
                                                                  waves, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC13WAVETYPE", "osc 1x3 wave type",
                                                                  waves, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC21WAVETYPE", "osc 2x1 wave type",
                                                                  waves, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC22WAVETYPE", "osc 2x2 wave type",
                                                                  waves, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC23WAVETYPE", "osc 2x3 wave type",
                                                                  waves, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC31WAVETYPE", "osc 3x1 wave type",
                                                                  waves, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC32WAVETYPE", "osc 3x2 wave type",
                                                                  waves, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC33WAVETYPE", "osc 3x3 wave type",
                                                                  waves, 0));
    
    // CHOOSE THE PITCH OF A SPECIFIC OSCILLATOR
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC11PITCH", "osc 1x1 pitch", juce::NormalisableRange<float> { -2400.0f, 2400.0f, 0.1f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC12PITCH", "osc 1x2 pitch", juce::NormalisableRange<float> { -2400.0f, 2400.0f, 0.1f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC13PITCH", "osc 1x3 pitch", juce::NormalisableRange<float> { -2400.0f, 2400.0f, 0.1f }, 0.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC21PITCH", "osc 2x1 pitch", juce::NormalisableRange<float> { -2400.0f, 2400.0f, 0.1f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC22PITCH", "osc 2x2 pitch", juce::NormalisableRange<float> { -2400.0f, 2400.0f, 0.1f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC23PITCH", "osc 2x3 pitch", juce::NormalisableRange<float> { -2400.0f, 2400.0f, 0.1f }, 0.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC31PITCH", "osc 3x1 pitch", juce::NormalisableRange<float> { -2400.0f, 2400.0f, 0.1f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC32PITCH", "osc 3x2 pitch", juce::NormalisableRange<float> { -2400.0f, 2400.0f, 0.1f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC33PITCH", "osc 3x3 pitch", juce::NormalisableRange<float> { -2400.0f, 2400.0f, 0.1f }, 0.0f));
    
    // CHOOSE LFO PARAMS
    params.push_back(std::make_unique<juce::AudioParameterChoice>("LFO1WAVETYPE", "Lfo 1 wave type",
                                                                  waves, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LFO1DEPTH", "lfo 1 depth", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.001f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LFO1FREQUENCY", "lfo 1 frequency", juce::NormalisableRange<float> { 0.0f, 10.0f, 0.001f }, 0.0f));
    
    // CHOOSE FILTER PARAMS FOR ROW OF OSCILLATORS RESONANCE
    juce::StringArray filterType = {"NONE", "LOWPASS", "HIGHPASS", "BANDPASS" };
    
    
    // FILTER 1 AKA ROW 1
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC1FILTERTYPE", "filter type",
                                                                  filterType, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC1CUTOFF", "cutoff", juce::NormalisableRange<float> { 0.0f, 20000.0f, 0.01f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC1RESONANCE", "resonance", juce::NormalisableRange<float> { 0.1f, 12.0f, 0.01f }, 0.2f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC1ATTACK", "attack", juce::NormalisableRange<float> { 0.0f, 5.0f, 0.01f }, 0.01));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC1DECAY", "decay", juce::NormalisableRange<float> { 0.0f, 5.0f, 0.01f }, 0.01));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC1SUSTAIN", "sustain", juce::NormalisableRange<float> { 0.0f, 5.0f, 0.01f }, 0.01));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC1RELEASE", "release", juce::NormalisableRange<float> { 0.0f, 5.0f, 0.01f }, 0.01));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC1GAIN", "gain", juce::NormalisableRange<float> { -50.0f, 6.0f, 0.01f }, -3.0f));
    
    // FILTER 2 AKA ROW 2
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC2FILTERTYPE", "filter type",
                                                                  filterType, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC2CUTOFF", "cutoff", juce::NormalisableRange<float> { 0.0f, 20000.0f, 0.01f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC2RESONANCE", "resonance", juce::NormalisableRange<float> { 0.1f, 12.0f, 0.01f }, 0.2f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC2ATTACK", "attack", juce::NormalisableRange<float> { 0.0f, 5.0f, 0.01f }, 0.01));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC2DECAY", "decay", juce::NormalisableRange<float> { 0.0f, 5.0f, 0.01f }, 0.01));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC2SUSTAIN", "sustain", juce::NormalisableRange<float> { 0.0f, 5.0f, 0.01f }, 0.01));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC2RELEASE", "release", juce::NormalisableRange<float> { 0.0f, 5.0f, 0.01f }, 0.01));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC2GAIN", "gain", juce::NormalisableRange<float> { -50.0f, 6.0f, 0.01f }, -3.0f));
    
    // FILTER 3 AKA ROW 3
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC3FILTERTYPE", "filter type",
                                                                  filterType, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC3CUTOFF", "cutoff", juce::NormalisableRange<float> { 0.0f, 20000.0f, 0.01f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC3RESONANCE", "resonance", juce::NormalisableRange<float> { 0.1f, 12.0f, 0.01f }, 0.2f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC3ATTACK", "attack", juce::NormalisableRange<float> { 0.0f, 5.0f, 0.01f }, 0.01));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC3DECAY", "decay", juce::NormalisableRange<float> { 0.0f, 5.0f, 0.01f }, 0.01));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC3SUSTAIN", "sustain", juce::NormalisableRange<float> { 0.0f, 5.0f, 0.01f }, 0.01));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC3RELEASE", "release", juce::NormalisableRange<float> { 0.0f, 5.0f, 0.01f }, 0.01));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OSC3GAIN", "gain", juce::NormalisableRange<float> { -50.0f, 6.0f, 0.01f }, -3.0f));
    
    // CHOOSE IF OSCILLATOR IS ACTIVE OR NOT
    params.push_back(std::make_unique<juce::AudioParameterBool>("OSC11ACTIVE", "osc active", true));
    params.push_back(std::make_unique<juce::AudioParameterBool>("OSC12ACTIVE", "osc active", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("OSC13ACTIVE", "osc active", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("OSC21ACTIVE", "osc active", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("OSC22ACTIVE", "osc active", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("OSC23ACTIVE", "osc active", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("OSC31ACTIVE", "osc active", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("OSC32ACTIVE", "osc active", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("OSC33ACTIVE", "osc active", false));
    
    // CUSTOM OSCILLATOR PARAMS
    params.push_back(std::make_unique<juce::AudioParameterChoice>("CUSTOMOSCFUNCTIONTYPE", "function type",
                                                                  juce::StringArray {"COSINE", "SINE"}, 0));
    params.push_back(std::make_unique<juce::AudioParameterInt>("CUSTOMOSCACCURACY", "accuracy", 1, 10, 1));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("CUSTOMOSCMUL", "multiply val", juce::NormalisableRange<float> { 1.0f, 5.0f, 0.01f }, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("CUSTOMOSCREPEATN", "repeat N", juce::NormalisableRange<float> { 1.0f, 5.0f, 1.0f }, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("CUSTOMOSCREPEATX", "repeat X", juce::NormalisableRange<float> { 1.0f, 5.0f, 1.0f }, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("CUSTOMOSCPITCH", "custom pitch", juce::NormalisableRange<float> { -2400.0f, 2400.0f, 0.1f }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterBool>("CUSTOMOSCENABLED", "custom osc enabled", false));
    
    // MASTER ADSR PARAMS
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MASTERATTACK", "attack", juce::NormalisableRange<float> { 0.0f, 5.0f, 0.01f }, 0.01));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MASTERDECAY", "decay", juce::NormalisableRange<float> { 0.0f, 5.0f, 0.01f }, 0.01));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MASTERSUSTAIN", "sustain", juce::NormalisableRange<float> { 0.0f, 5.0f, 0.01f }, 0.01));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MASTERRELEASE", "release", juce::NormalisableRange<float> { 0.0f, 5.0f, 0.01f }, 0.01));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MASTERGAIN", "gain", juce::NormalisableRange<float> { -50.0f, 6.0f, 0.01f }, -3.0f));
    
    // REVERB PARAMS
    params.push_back(std::make_unique<juce::AudioParameterFloat>("REVERBROOMSIZE", "room size", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 0.00));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("REVERBDAMPING", "damping", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 0.00));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("REVERBWETLEVEL", "wet level",juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 0.00));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("REVERBDRYLEVEL", "dry level", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 0.00));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("REVERBWIDTH", "width", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 0.00));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("REVERBFREEZEMODE", "freeze mode", juce::NormalisableRange<float> { 0.0f, 0.5f, 0.005f }, 0.00));
    params.push_back(std::make_unique<juce::AudioParameterBool>("REVERBACTIVE", "reverb active", false));
    
    // CHORUS PARAMS
    params.push_back(std::make_unique<juce::AudioParameterFloat>("CHORUSRATE", "rate", juce::NormalisableRange<float> { 0.0f, 10.0f, 0.01f }, 0.00));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("CHORUSDEPTH", "depth", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 0.00));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("CHORUSCENTREDELAY", "centre delay",juce::NormalisableRange<float> { 0.0f, 1000.0f, 0.1f }, 0.00));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("CHORUSFEEDBACK", "feedback", juce::NormalisableRange<float> { -1.0f, 1.0f, 0.01f }, 0.00));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("CHORUSMIX", "mix", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 0.00));
    params.push_back(std::make_unique<juce::AudioParameterBool>("CHORUSACTIVE", "chorus active", false));
    return {params.begin(), params.end()};
}
