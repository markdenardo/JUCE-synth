/*
  ==============================================================================

    SynthSound.h
    Created: 19 Apr 2021 7:52:57pm
    Author:  MDN

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
    bool     appliesToNote (int midiNoteNumber) override { return true; }
    bool     appliesToChannel (int midiChannel) override { return true; }
    
};
 
