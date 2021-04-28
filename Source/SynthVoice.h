/*
  ==============================================================================

    SynthVoice.h
    Created: 19 Apr 2021 7:52:31pm
    Author:  MDN

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels);
    void renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples) override;
private:
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;
    
    
    juce::dsp::Oscillator<float> osc { [](float x) {return x/juce::MathConstants<float>::pi;}};
    juce::dsp::Gain<float>gain;
    bool isPrepared { false };
    
    // return std::sin (x); //Sine Wave
    // return x / MathConstants<float>::pi; // Saw Wave
    // return x < 0.0f ? -1.0f : 1.0f;  // Square Wave
};
