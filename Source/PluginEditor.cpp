/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FiltroAudioProcessorEditor::FiltroAudioProcessorEditor(FiltroAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), spectrumAnalizer(13, 8192)
{
    addAndMakeVisible(spectrumAnalizer);
    // Parametros de Sliders
    FcLp.setRange(20, 2000);
    setSliderWithLabel(FcLp, LpLabel, audioProcessor.apvts, "LOWCUTFREQ", FcLpAttachement);

    setSliderWithLabel(FcHp,
        HpLabel,
        audioProcessor.apvts, "HIGHCUTFREQ", FcHpAttachement);
    setSliderWithLabel(FcB1,
        B1Label,
        audioProcessor.apvts, "BELLFREQ", FcB1Attachement);

    setSliderWithLabel(FcB2,
        B2Label,
        audioProcessor.apvts, "BELLFREQ2", FcB2Attachement);

    setSliderWithLabel(FcSL,
        ShelfLPLabel, audioProcessor.apvts, "SHELFFREQ", FCSLAttachement);

    setSliderWithLabel(FcSH, 
        ShelfHPLabel, audioProcessor.apvts, "SHELFHFREQ", FcSHAttachement);

    setSliderWithLabel(Gain,
        G,
        audioProcessor.apvts, "BELLGAIN", FcBgainAttachement);

    setSliderWithLabel(Gain2,
        G2,
        audioProcessor.apvts, "BELLGAIN2", FcBgain2Attachement);

    setSliderWithLabel(Gain3,
        G3,
        audioProcessor.apvts, "SHELFGAIN", SLgain3Attachement);

    setSliderWithLabel(Gain4,
        G4,
        audioProcessor.apvts, "SHELFHGAIN", SHgain4Attachement);

    setSliderWithLabel(Qslider,
        Q,
        audioProcessor.apvts,
        "BELLQ", QBAttachement);

    setSliderWithLabel(Q2slider,
        Q2,
        audioProcessor.apvts,
        "BELLQ2",
        QB2Attachement);
    setSliderWithLabel(Q3slider,
        Q3,
        audioProcessor.apvts, "SHELFQ", QS3Attachement);
    setSliderWithLabel(Q4slider,
        Q4,
        audioProcessor.apvts, "SHELFHQ", QS4Attachement);

    //Parametros ComboBox
    setComboBoxWithLabel(SlopeLp,
        SlopeSelector,
        audioProcessor.apvts, "LOWCUTSLOPE", SlopeLpAttachement);
    setComboBoxWithLabel(SlopeHp,
        SlopeSelectorHp,
        audioProcessor.apvts, "HIGHCUTSLOPE", SlopeHpAttachement);

    SetTogleButton(LpBypassed, audioProcessor.apvts, "LPBYPASS", LpBypassedAttachement);
    SetTogleButton(HpBypassed, audioProcessor.apvts, "HPBYPASS", HpBypassedAttachement);
    SetTogleButton(B1Bypassed, audioProcessor.apvts, "B1BYPASS", B1BypassedAttachement);
    SetTogleButton(B2Bypassed, audioProcessor.apvts, "B2BYPASS", B2BypassedAttachement);
    SetTogleButton(SlBypassed, audioProcessor.apvts, "SLBYPASS", SlBypassedAttachement );
    SetTogleButton(ShBypassed, audioProcessor.apvts, "SHBYPASS", ShBypassedAttachement);

    Setlabel(TiTle, "T");
    Setlabel(info, "H");

    openGLC.attachTo(spectrumAnalizer);

    setSize(1050, 750);
    startTimerHz(60);
}

FiltroAudioProcessorEditor::~FiltroAudioProcessorEditor()
{
    openGLC.detach();
}

//==============================================================================
void FiltroAudioProcessorEditor::paint (juce::Graphics& g)
{
    
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);
    g.setColour(juce::Colours::turquoise);
   ;
    



    auto bounds = getLocalBounds();
    auto responseArea = bounds.removeFromTop(bounds.getHeight() * 0.33333 + 1);

    auto w = responseArea.getWidth();


}

void FiltroAudioProcessorEditor::resized()
{
    const auto x = 180;
    const auto xLH = 30;
    const auto yLH = 20;
    const auto y = 220;
    const auto sliderWidth  = 100;
    const auto sliderHeight = 115;
    const auto BoxWidth = 100;
    const auto BoxHeight = 50;
    const auto Offset =  160;
    const auto labelOffset = 27;
    const auto LabelHeight = 50;
    const auto padding = 200;
    const auto Togle = 50;
    const auto TogleOffset = 27*2;

    
    //Labels
    TiTle.setBounds(870 ,620, 150, 200);
    


    //Sliders Bounds
    FcLp.setBounds(xLH, yLH, sliderWidth, sliderHeight);
    LpLabel.setBounds(FcLp.getX(), FcLp.getY() - labelOffset, sliderWidth,LabelHeight);
    
    
    FcSL.setBounds(x, y, sliderWidth, sliderHeight);
    ShelfLPLabel.setBounds(FcSL.getX(), FcSL.getY() - labelOffset, sliderWidth, LabelHeight);
    FcSH.setBounds(x + (padding * 3) - 30, y, sliderWidth, sliderHeight);
    ShelfHPLabel.setBounds(FcSH.getX(), FcSH.getY() - labelOffset, sliderWidth, LabelHeight);



    FcB1.setBounds(x + padding , y, sliderWidth, sliderHeight);
    B1Label.setBounds(FcB1.getX(), FcB1.getY() - labelOffset, sliderWidth, LabelHeight);
    FcB2.setBounds(x + (padding * 2), y, sliderWidth, sliderHeight);
    B2Label.setBounds(FcB2.getX(), FcB2.getY() - labelOffset, sliderWidth, LabelHeight);




    Gain.setBounds(FcB1.getX(), FcB1.getY() + Offset, sliderWidth, sliderHeight);
    G.setBounds(Gain.getX(), Gain.getY() - labelOffset, sliderWidth, LabelHeight);
    Gain2.setBounds(FcB2.getX(), FcB2.getY() + Offset, sliderWidth, sliderHeight);
    G2.setBounds(Gain2.getX(), Gain2.getY() - labelOffset, sliderWidth, LabelHeight);
    Gain3.setBounds(FcSL.getX(), FcSL.getY() + Offset, sliderWidth, sliderHeight);
    G3.setBounds(Gain3.getX(), Gain3.getY() - labelOffset, sliderWidth, LabelHeight);
    Gain4.setBounds(FcSH.getX(), FcSH.getY() + Offset, sliderWidth, sliderHeight);
    G4.setBounds(Gain4.getX(), Gain4.getY() - labelOffset, sliderWidth, LabelHeight);
    
    Qslider.setBounds(Gain.getX(), Gain.getY() + Offset, sliderWidth, sliderHeight);
    Q.setBounds(Qslider.getX(), Qslider.getY() - labelOffset, sliderWidth, LabelHeight);
    Q2slider.setBounds(Gain2.getX(), Gain2.getY() + Offset, sliderWidth, sliderHeight);
    Q2.setBounds(Q2slider.getX(), Q2slider.getY() - labelOffset, sliderWidth, LabelHeight);
    Q3slider.setBounds(Gain3.getX(), Gain3.getY() + Offset, sliderWidth, sliderHeight);
    Q3.setBounds(Q3slider.getX(), Q3slider.getY() - labelOffset, sliderWidth, LabelHeight);
    Q4slider.setBounds(Gain4.getX(), Gain4.getY() + Offset, sliderWidth, sliderHeight);
    Q4.setBounds(Q4slider.getX(), Q4slider.getY() - labelOffset, sliderWidth, LabelHeight);

    
    B1Bypassed.setBounds(Qslider.getX() + 36, Qslider.getY() + TogleOffset*2, Togle, Togle);
    B2Bypassed.setBounds(Q2slider.getX() + 38, Q2slider.getY() + TogleOffset*2, Togle, Togle);
    SlBypassed.setBounds(Q3slider.getX() + 38, Q3slider.getY() + TogleOffset * 2, Togle, Togle);
    ShBypassed.setBounds(Q4slider.getX() + 38, Q4slider.getY() + TogleOffset * 2, Togle, Togle);

    FcHp.setBounds(xLH + (padding * 4) + 65, yLH
         , sliderWidth, sliderHeight);
    HpLabel.setBounds(FcHp.getX(), FcHp.getY() - labelOffset, sliderWidth, LabelHeight);
    
    spectrumAnalizer.setBounds(200, 0, 620, 200);





    const auto settings =  36;
    //Comobox bounds
    SlopeLp.setBounds(FcLp.getX(), FcLp.getY() + Offset - 10, BoxWidth, BoxHeight);
    SlopeSelector.setBounds(SlopeLp.getX(), SlopeLp.getY() - labelOffset-10, sliderWidth, LabelHeight);
    LpBypassed.setBounds(SlopeLp.getX()+ settings, SlopeLp.getY() + TogleOffset, Togle, Togle);

    SlopeHp.setBounds(FcHp.getX(), FcHp.getY() + Offset - 10, BoxWidth, BoxHeight);
    SlopeSelectorHp.setBounds(SlopeHp.getX(), SlopeHp.getY() - labelOffset-10, sliderWidth, LabelHeight);
    HpBypassed.setBounds(SlopeHp.getX()+ settings, SlopeHp.getY() + TogleOffset, Togle, Togle);




}

void FiltroAudioProcessorEditor::timerCallback()
{
    if (audioProcessor.lleno)
    {
        spectrumAnalizer.receiveAndProcess(audioProcessor.bufferCirc.getRawDataPointer());
        audioProcessor.lleno = false;
    }
}

void FiltroAudioProcessorEditor::setSliderWithLabel(juce::Slider& slider, juce::Label& Label, juce::AudioProcessorValueTreeState& apvts, juce::String paramid, std::unique_ptr<Attachement>& attachment)
{
    
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId,juce::Colours::green);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(slider);

    attachment = std::make_unique< juce::AudioProcessorValueTreeState::SliderAttachment>
        (apvts, paramid, slider);

    Label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    Label.setFont(15.0f);
    Label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(Label);

}

void FiltroAudioProcessorEditor::setComboBoxWithLabel(juce::ComboBox& combox, juce::Label& Label, juce::AudioProcessorValueTreeState& apvts, juce::String paramid, std::unique_ptr<AttachementC>& attachment)
{
    juce::StringArray choices{ "12 db/Oct", "24 db / Oct", "36 db/Oct" };
    combox.addItemList(choices, 1);
    addAndMakeVisible(combox);

    attachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>
        (apvts,paramid,combox);

    Label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    Label.setFont(15.0f);
    Label.setJustificationType(juce::Justification::left);
    addAndMakeVisible(Label);


}

void FiltroAudioProcessorEditor::SetTogleButton(juce::ToggleButton& Button, juce::AudioProcessorValueTreeState& apvts, juce::String paramid, std::unique_ptr<AttachementT>& attachment)
{
    addAndMakeVisible(Button);

    attachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, paramid, Button);
}

void FiltroAudioProcessorEditor::Setlabel(juce::Label& Label, juce::String id)
{
    if (id == "T")

    {
        Label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::sienna);
        Label.setFont(juce::Font(25.0f, juce::Font::bold));
        Label.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(Label);
    }
    else if (id == "H")
    {
        Label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::sienna);
        Label.setFont(juce::Font(18.0f, juce::Font::bold));
        Label.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(Label);
    }
}
