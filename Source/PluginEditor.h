/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "SpectrumClass.h"

//==============================================================================
/**
*/
class FiltroAudioProcessorEditor : public juce::AudioProcessorEditor,
                                   private juce::Timer
{
public:
    FiltroAudioProcessorEditor(FiltroAudioProcessor&);
    ~FiltroAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.

    SpectrumClass spectrumAnalizer;

    juce::Slider FcLp, FcHp, FcB1, FcB2, FcB3, FcSL, FcSH, Gain, Gain2, Gain3, Gain4, Qslider, Q2slider, Q3slider, Q4slider;
    juce::ComboBox SlopeLp, SlopeHp;
    using ComboxAttachement = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    using uptrc = std::unique_ptr<ComboxAttachement>;

    uptrc SlopeLpAttachement;
    uptrc SlopeHpAttachement;

    using SliderAttachement = juce::AudioProcessorValueTreeState::SliderAttachment;
    using uptr = std::unique_ptr<SliderAttachement>;
    uptr FcLpAttachement;
    uptr FcHpAttachement;
    uptr FcB1Attachement;
    uptr FcB2Attachement;
    uptr FcB3Attachement;

    uptr FcBgainAttachement;
    uptr FcBgain2Attachement;
    uptr SLgain3Attachement;
    uptr SHgain4Attachement;

    uptr FcSHAttachement;
    uptr FCSLAttachement;


    uptr QBAttachement;
    uptr QB2Attachement;
    uptr QS3Attachement;
    uptr QS4Attachement;

    juce::Label LpLabel{ "Low Pass Freq", "Low Pass Freq" };
    juce::Label HpLabel{ "High Pass Freq", "High Pass Freq" };
    juce::Label B1Label{ "Bell Pass Freq", "Bell Pass Freq" };
    juce::Label B2Label{ "Bell Pass Freq", "Bell Pass Freq" };
    juce::Label ShelfLPLabel{ "Bell Pass Freq", "Bell Pass Freq" };
    juce::Label ShelfHPLabel{ "Bell Pass Freq", "Bell Pass Freq" };
    juce::Label G{ "Gain", "Gain" };
    juce::Label G2{ "Gain", "Gain" };
    juce::Label G3{ "Gain", "Gain" };
    juce::Label G4{ "Gain", "Gain" };
    juce::Label Q{ "Q","Q" };
    juce::Label Q2{ "Q" , "Q" };
    juce::Label Q3{ "Q" , "Q" };
    juce::Label Q4{ "Q" , "Q" };
    juce::Label TiTle{ "FACK FILTER" , "FACK FILTER" };
    juce::Label info{ "Re alpha version" , "Re alpha version" };
    

    juce::Label SlopeSelector{ "Slope", "Slope" };
    juce::Label SlopeSelectorHp{ "Slope", "Slope" };
    

    using Attachement = juce::AudioProcessorValueTreeState::SliderAttachment;
    using AttachementC = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

    void setSliderWithLabel(juce::Slider& slider, juce::Label& Label, juce::AudioProcessorValueTreeState& apvts, juce::String paramid, std::unique_ptr<Attachement>& attachment);
    void setComboBoxWithLabel(juce::ComboBox& combox, juce::Label& Label, juce::AudioProcessorValueTreeState& apvts, juce::String paramid, std::unique_ptr<AttachementC>& attachment);

    juce::ToggleButton LpBypassed,
        B1Bypassed,
        B2Bypassed,
        HpBypassed,
        ShBypassed,
        SlBypassed;

    using TogleAttachement = juce::AudioProcessorValueTreeState::ButtonAttachment;
    using uptrT = std::unique_ptr<TogleAttachement>;

    uptrT LpBypassedAttachement,
        B1BypassedAttachement,
        B2BypassedAttachement,
        HpBypassedAttachement,
        ShBypassedAttachement,
        SlBypassedAttachement;

    using AttachementT = juce::AudioProcessorValueTreeState::ButtonAttachment;
    void SetTogleButton(juce::ToggleButton& Button, juce::AudioProcessorValueTreeState& apvts, juce::String paramid, std::unique_ptr<AttachementT>& attachment);
    void Setlabel(juce::Label& Label, juce::String id);

    juce::OpenGLContext openGLC;

    FiltroAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FiltroAudioProcessorEditor)
};
