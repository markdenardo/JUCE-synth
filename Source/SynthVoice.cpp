/*
  ==============================================================================

    SynthVoice.cpp
    Created: 19 Apr 2021 7:52:31pm
    Author:  MDN

  ==============================================================================
*/

#include "SynthVoice.h"
bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) !=nullptr;
}
void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)
{
    osc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    adsr.noteOn();
}
void SynthVoice::stopNote (float velocity, bool allowTailOff)
{
    adsr.noteOff();
}
void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue)
{
    
}
void SynthVoice::pitchWheelMoved (int newPitchWheelValue)
{
    
}
void SynthVoice::prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels)
{
    adsr.setSampleRate(sampleRate); 
    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
    
    osc.prepare(spec);
    gain.prepare(spec);
    

    gain.setGainLinear(0.01f);
    
    adsrParams.attack = 0.8f;
    adsrParams.decay = 0.8f;
    adsrParams.sustain = 1.0f;
    adsrParams.release = 1.5f;
    
    adsr.setParameters(adsrParams);
    
    isPrepared = true;
}
void SynthVoice::renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples)
{
    jassert(isPrepared);
    
    if (! isVoiceActive())
        return;
    
    synthBuffer.setSize (outputBuffer.getNumChannels(),numSamples,false,false,true);
    synthBuffer.clear();
    
    juce::dsp::AudioBlock<float> audioBlock {outputBuffer};
    osc.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    
    adsr.applyEnvelopeToBuffer(outputBuffer, 0, synthBuffer.getNumSamples());
    
    for (int channel =0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);
        
        if (! adsr.isActive())
            clearCurrentNote();
    }
}
