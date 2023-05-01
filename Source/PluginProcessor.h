/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Filtro.h"
#include "FilterCreator.h"

//==============================================================================
/**
*/
class FiltroAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    FiltroAudioProcessor();
    ~FiltroAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    

    
    Filtro F_LP2, F_LP4, F_LP6;
    juce::AudioProcessorValueTreeState apvts;
    std::atomic<bool> lleno;
    juce::Array<float> bufferCirc;
    int cont = 0;
private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    FilterCreator LowpassL,
        LowpassR, 
        HighpassL, 
        HighpassR, 
        BellL, 
        BellR, 
        Bell1L, 
        Bell1R,
        ShelfHL,
        ShelfHR,
        ShelfLL,
        ShelfLR;
    int slope = 1;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FiltroAudioProcessor)
};
