# Welcome to MySynth!

<img width="550" height="400" alt="Screenshot 2025-07-17 at 6 05 20 PM" src="https://github.com/user-attachments/assets/102f9657-7b0a-49fe-8c41-dba90bacfbef" />

## Description

**Overview:**  
MySynth is an audio plugin developed in C++ utilizing the JUCE code library. It has a variety of features, including 9 separate oscillators for additive synthesis, filters for subtractive synthesis, and audio effects such as a reverb and chorus. All of these features are controlled by their ADSR to further diversify the possibilities for sound creation.

---

## Layout

### Oscillators (middle left in 3×3 grid)  
This synthesizer has 9 separate oscillators, which the user can combine to create a variety of effects. Each oscillator has its own controls, including a wave type selector, a detune knob, and the ability to enable or disable each oscillator.

### Oscillator Controls (to the right of each row)  
Each row of oscillators contains a filter and an ADSR for the filter. Each filter contains three separate filter types (lowpass, highpass, bandpass) and knobs for the cutoff and resonance. Each of these filters is connected to an ADSR (to the right) as well as volume control for that row.

### Custom Oscillator (bottom left)  
This synth also contains one extra oscillator that utilizes a Fourier transform to create a unique sound. The user has access to the same parameters as any other oscillator (detune control, on-off), but with the added ability to customize the function through a variety of parameters.

---

## Effects

### Low Frequency Oscillator (Top middle)  
This synth contains a single LFO, which modulates the frequency of all the activated oscillators. The user has the ability to select the waveform for this oscillator, as well as its frequency and depth.

### Reverb (top right)  
This synth contains a reverb that controls the sound of the whole synth if activated. The user has control over a variety of different controls including size, damp level, wet level, dry level, width, and the amount of freeze to apply.

### Chorus (bottom right)  
Finally, this synth contains a chorus effect that alters the sound of the entire synth. Available controls include the depth, delay, feedback, mix, and rate of the chorus.
