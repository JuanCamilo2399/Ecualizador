/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FiltroAudioProcessor::FiltroAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                        ),
                        LowpassL(44100.0f, 1000.0f, 0.707f, 1, 1, 3),
                        LowpassR(44100.0f, 1000.0f, 0.707f, 1, 1, 3),
                        HighpassL(44100.0f, 1000.0f, 0.707f, 1, 1, 3),
                        HighpassR(44100.0f, 1000.0f, 0.707f, 1, 1, 3),
                        BellL(44100.0f, 1000.0f, 0.707f, 1, 1, 3),
                        BellR(44100.0f, 1000.0f, 0.707f, 1, 1, 3),
                        Bell1L(44100.0f, 1000.0f, 0.707f, 1, 1, 3),
                        Bell1R(44100.0f, 1000.0f, 0.707f, 1, 1, 3),
                        ShelfHL(44100.0f, 1000.0f, 0.707f, 1, 1, 3),
                        ShelfHR(44100.0f, 1000.0f, 0.707f, 1, 1, 3),
                        ShelfLL(44100.0f, 1000.0f, 0.707f, 1, 1, 3),
                        ShelfLR(44100.0f, 1000.0f, 0.707f, 1, 1, 3),
                        lleno(false),
                        apvts (*this,nullptr,"Parameters",createParameters())
                          
                               
#endif
{
    bufferCirc.resize(2048 * 4);
    juce::zeromem(bufferCirc.getRawDataPointer(), sizeof(float) * 2048 * 4);
}

FiltroAudioProcessor::~FiltroAudioProcessor()
{
}

//==============================================================================
const juce::String FiltroAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FiltroAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FiltroAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FiltroAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FiltroAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FiltroAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FiltroAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FiltroAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FiltroAudioProcessor::getProgramName (int index)
{
    return {};
}

void FiltroAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FiltroAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{


    LowpassL.CreateFilter(0, 3);
    LowpassR.CreateFilter(0, 3);
    HighpassL.CreateFilter(1, 3);
    HighpassR.CreateFilter(1, 3);
    BellL.CreateFilter(2, 3);
    BellR.CreateFilter(2, 3);
    Bell1L.CreateFilter(2, 3);
    Bell1R.CreateFilter(2, 3);
    ShelfLL.CreateFilter(2, 3);
    ShelfLR.CreateFilter(2, 3);
    ShelfHL.CreateFilter(2, 3);
    ShelfHR.CreateFilter(2, 3);

    LowpassL.updateParameters(sampleRate, "Fs");
    LowpassR.updateParameters(sampleRate, "Fs");
    HighpassL.updateParameters(sampleRate, "Fs");
    HighpassR.updateParameters(sampleRate, "Fs");
    BellL.updateParameters(sampleRate, "Fs");
    BellR.updateParameters(sampleRate, "Fs");
    Bell1L.updateParameters(sampleRate, "Fs");
    Bell1R.updateParameters(sampleRate, "Fs");
    ShelfLL.updateParameters(sampleRate, "Fs");
    ShelfLR.updateParameters(sampleRate, "Fs");
    ShelfHL.updateParameters(sampleRate, "Fs");
    ShelfHR.updateParameters(sampleRate, "Fs");


}

void FiltroAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FiltroAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void FiltroAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    //Lp Parameters
    auto Fclp = apvts.getRawParameterValue("LOWCUTFREQ");
    auto Ordenlp = apvts.getRawParameterValue("LOWCUTSLOPE");
    auto Statelp = apvts.getRawParameterValue("LPBYPASS");
   
    LowpassL.updateParameters(Fclp->load(), "Fc");
    LowpassL.updateSlope(Ordenlp->load());
    LowpassL.OnOffEq(Statelp->load());
    LowpassR.updateParameters(Fclp->load(), "Fc");
    LowpassR.updateSlope(Ordenlp->load());
    LowpassR.OnOffEq(Statelp->load());

    auto FcHp = apvts.getRawParameterValue("HIGHCUTFREQ");
    auto OrdenHp = apvts.getRawParameterValue("HIGHCUTSLOPE");
    auto StateHp = apvts.getRawParameterValue("HPBYPASS");
    HighpassL.updateParameters(FcHp->load(), "Fc");
    HighpassL.updateSlope(OrdenHp->load());
    HighpassL.OnOffEq(StateHp->load());
    HighpassR.updateParameters(FcHp->load(), "Fc");
    HighpassR.updateSlope(OrdenHp->load());
    HighpassR.OnOffEq(StateHp->load());

    //Bell parameters
    auto FcB = apvts.getRawParameterValue("BELLFREQ");
    auto Gain = apvts.getRawParameterValue("BELLGAIN");
    auto Q = apvts.getRawParameterValue("BELLQ");
    auto StateB = apvts.getRawParameterValue("B1BYPASS");
    BellL.updateParameters(FcB->load(), "Fc");
    BellL.updateParameters(Gain->load(), "V0");
    BellL.updateParameters(Q->load(), "Q");
    BellL.OnOffEq(StateB->load());
    BellR.updateParameters(FcB->load(), "Fc");
    BellR.updateParameters(Gain->load(), "V0");
    BellR.updateParameters(Q->load(), "Q");
    BellR.OnOffEq(StateB->load());


    auto FcB1 = apvts.getRawParameterValue("BELLFREQ2");
    auto Gain1 = apvts.getRawParameterValue("BELLGAIN2");
    auto Q1 = apvts.getRawParameterValue("BELLQ2");
    auto StateB1 = apvts.getRawParameterValue("B2BYPASS");
    Bell1L.updateParameters(FcB1->load(), "Fc");
    Bell1L.updateParameters(Gain1->load(), "V0");
    Bell1L.updateParameters(Q1->load(), "Q");
    Bell1L.OnOffEq(StateB1->load());
    Bell1R.updateParameters(FcB1->load(), "Fc");
    Bell1R.updateParameters(Gain1->load(), "V0");
    Bell1R.updateParameters(Q1->load(), "Q");
    Bell1R.OnOffEq(StateB1->load());
     

    //Shelf Parameters
    auto FcSL = apvts.getRawParameterValue("SHELFFREQ");
    auto GainSL = apvts.getRawParameterValue("SHELFGAIN");
    auto QSL = apvts.getRawParameterValue("SHELFQ");
    auto StateSL = apvts.getRawParameterValue("SLBYPASS");
    ShelfLL.updateParameters(FcSL->load(), "Fc");
    ShelfLL.updateParameters(GainSL->load(), "V0");
    ShelfLL.updateParameters(QSL->load(), "Q");
    ShelfLL.OnOffEq(StateSL->load());
    ShelfLR.updateParameters(FcSL->load(), "Fc");
    ShelfLR.updateParameters(GainSL->load(), "V0");
    ShelfLR.updateParameters(QSL->load(), "Q");
    ShelfLR.OnOffEq(StateSL->load());



    auto FcSH = apvts.getRawParameterValue("SHELFHFREQ");
    auto GainSH = apvts.getRawParameterValue("SHELFHGAIN");
    auto QSH = apvts.getRawParameterValue("SHELFHQ");
    auto StateSH = apvts.getRawParameterValue("SHBYPASS");
    ShelfHL.updateParameters(FcSH->load(), "Fc");
    ShelfHL.updateParameters(GainSH->load(), "V0");
    ShelfHL.updateParameters(QSH->load(), "Q");
    ShelfHL.OnOffEq(StateSH->load());
    ShelfHR.updateParameters(FcSH->load(), "Fc");
    ShelfHR.updateParameters(GainSH->load(), "V0");
    ShelfHR.updateParameters(QSH->load(), "Q");
    ShelfHR.OnOffEq(StateSH->load());




    for (int sample = 0; sample < buffer.getNumSamples(); sample++)
    {
        for (int channel = 0; channel < totalNumInputChannels; channel++)
        {
            float* sampleN = buffer.getWritePointer(channel);
            if (channel == 0)
            {
                sampleN[sample] = LowpassL.processSample(sampleN[sample]);
                sampleN[sample] = HighpassL.processSample(sampleN[sample]);
                sampleN[sample] = BellL.processSample(sampleN[sample]);
                sampleN[sample] = Bell1L.processSample(sampleN[sample]);
                
                sampleN[sample] = ShelfHL.processSample(sampleN[sample]);
                sampleN[sample] = ShelfLL.processSample(sampleN[sample]);

                float valueCopy = sampleN[sample];

                bufferCirc.set(cont, valueCopy);

                cont++;

                if (cont >= 2048)
                { lleno = true; }
                
                if (cont >= (2048 * 4))
                {
                    cont = 0;
                }
            }
            else if (channel == 1)
            {
                sampleN[sample] = LowpassR.processSample(sampleN[sample]);
                sampleN[sample] = HighpassR.processSample(sampleN[sample]);
                sampleN[sample] = BellR.processSample(sampleN[sample]);
                sampleN[sample] = Bell1R.processSample(sampleN[sample]);
                
                sampleN[sample] = ShelfHR.processSample(sampleN[sample]);
                sampleN[sample] = ShelfLR.processSample(sampleN[sample]);
            }
        }
    }
}

//==============================================================================
bool FiltroAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FiltroAudioProcessor::createEditor()
{

    return new FiltroAudioProcessorEditor (*this);
    
}

//==============================================================================
void FiltroAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FiltroAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

juce::AudioProcessorValueTreeState::ParameterLayout FiltroAudioProcessor::createParameters()
{
    //Creación de parametros para Low Pass, High Pass, Bell
    //Para Lpass y Hpass Fc,Pendiente.
    //Para Bell Fc,Gain.
    
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> Parameters;

    //Parametros de Sliders
    Parameters.push_back(std::make_unique<juce::AudioParameterFloat>("LOWCUTFREQ",
        "Lowcut Freq", juce::NormalisableRange<float>{20.0f, 20000.0f, 0.1f, 0.25f}, 15000.0f));
    Parameters.push_back(std::make_unique<juce::AudioParameterFloat>("HIGHCUTFREQ", 
        "Highcut Freq", juce::NormalisableRange<float>{20.0f, 20000.0f, 0.1f, 0.25f}, 20.0f));

    Parameters.push_back(std::make_unique<juce::AudioParameterFloat>("SHELFFREQ",
        "Shelf Freq", juce::NormalisableRange<float>{20.0f, 20000.0f, 0.1f, 0.25f}, 20.0f));

    Parameters.push_back(std::make_unique<juce::AudioParameterFloat>("SHELFGAIN",
        "Shelf Gain", juce::NormalisableRange<float>{-24.0f, 24.0f, 0.01f, 1.f}, 0.0f));

    Parameters.push_back(std::make_unique<juce::AudioParameterFloat>("SHELFQ",
        "Shelf Q", juce::NormalisableRange<float>{0.024f, 40.0f, 0.1f, 0.25f}, 0.0f));

    Parameters.push_back(std::make_unique<juce::AudioParameterFloat>("SHELFHFREQ",
        "Shelfh Freq", juce::NormalisableRange<float>{20.0f, 20000.0f, 0.1f, 0.25f}, 15000.0f));

    Parameters.push_back(std::make_unique<juce::AudioParameterFloat>("SHELFHGAIN",
        "Shelfh Gain", juce::NormalisableRange<float>{-24.0f, 24.0f, 0.01f, 1.f}, 0.0f));

    Parameters.push_back(std::make_unique<juce::AudioParameterFloat>("SHELFHQ",
        "Shelfh Q", juce::NormalisableRange<float>{0.024f, 40.0f, 0.1f, 0.25f}, 0.0f));
    
     



    Parameters.push_back(std::make_unique<juce::AudioParameterFloat>("BELLFREQ",
        "Bell Freq", juce::NormalisableRange<float>{20.0f, 20000.0f, 0.1f, 0.25f}, 500.0f));
    Parameters.push_back(std::make_unique<juce::AudioParameterFloat>("BELLGAIN", 
        "Bell Gain", juce::NormalisableRange<float>{-24.0f, 24.0f, 0.01f, 1.f}, 0.0f));
    Parameters.push_back(std::make_unique<juce::AudioParameterFloat>("BELLQ",
        "Bell Q", juce::NormalisableRange<float>{0.024f, 40.0f, 0.1f, 0.25f}, 0.0f));


    Parameters.push_back(std::make_unique<juce::AudioParameterFloat>("BELLFREQ2",
        "Bell2 Freq", juce::NormalisableRange<float>{20.0f, 20000.0f, 0.1f, 0.25f}, 700.0f));
    Parameters.push_back(std::make_unique<juce::AudioParameterFloat>("BELLGAIN2",
        "Bell2 Gain", juce::NormalisableRange<float>{-24.0f, 24.0f, 0.01f, 1.f}, 0.0f));
    Parameters.push_back(std::make_unique<juce::AudioParameterFloat>("BELLQ2",
        "Bell Q2", juce::NormalisableRange<float>{0.024f, 40.0f, 0.1f, 0.25f}, 0.0f));


    //ComboBox
    Parameters.push_back(std::make_unique<juce::AudioParameterChoice>("LOWCUTSLOPE", 
        "LowCut slope", juce::StringArray{ "12 dB/Oct","24 dB/Oct","36 dB/Oct" }, 0,""));
    Parameters.push_back(std::make_unique<juce::AudioParameterChoice>("HIGHCUTSLOPE",
        "HighCut slope", juce::StringArray{ "12 dB/Oct","24 dB/Oct","36 dB/Oct" }, 0 ,""));

    Parameters.push_back(std::make_unique<juce::AudioParameterBool>("LPBYPASS",
        "Lp Bypass", false));
    Parameters.push_back(std::make_unique<juce::AudioParameterBool>("HPBYPASS",
        "Hp Bypass", false));
    Parameters.push_back(std::make_unique<juce::AudioParameterBool>("B1BYPASS",
        "B1 Bypass", false));
    Parameters.push_back(std::make_unique<juce::AudioParameterBool>("B2BYPASS",
        "B2 Bypass", false));
    Parameters.push_back(std::make_unique<juce::AudioParameterBool>("SHBYPASS",
        "SH Bypass", false));
    Parameters.push_back(std::make_unique<juce::AudioParameterBool>("SLBYPASS",
        "SL Bypass", false));



    return {Parameters.begin(),Parameters.end()};
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FiltroAudioProcessor();
}

