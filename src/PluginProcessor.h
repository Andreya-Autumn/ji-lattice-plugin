/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <set>
#include <atomic>

#include "JIMath.h"


class LatticesProcessor : public juce::AudioProcessor, juce::Timer, private juce::AudioProcessorParameter::Listener
{
public:
    //==============================================================================
  LatticesProcessor();
    ~LatticesProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    //==============================================================================
    
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    void timerCallback() override;
    
    void modeSwitch(int m);
    void updateMIDI(int wCC, int eCC, int nCC, int sCC, int hCC, int C);
    void updateFreq(double f);
    double updateRoot(int r);
    void parameterValueChanged(int parameterIndex, float newValue) override;
    
    bool registeredMTS{false};
    
    std::atomic<int> positionX{0};
    std::atomic<int> positionY{0};
    
    enum Mode
    {
        Pyth,
        Syntonic,
        Duodene,
    };
    std::atomic<Mode> mode = Duodene;
    std::atomic<bool> changed{false};
    std::atomic<int> numClients{0};
    
    std::pair<int, int> coOrds[12]{};
    
    int syntonicDrift = 0;
    int diesisDrift = 0;
    int pythDrift = 0;
    
    int shiftCCs[5] = {5, 6, 7, 8, 9};
    int listenOnChannel = 1;
    
    int originalRefNote{-12};
    double originalRefFreq{-1};
    
private:
    static constexpr int maxDistance{24};
    static constexpr int defaultRefNote{0};
    static constexpr double defaultRefFreq{261.6255653005986};
    
    juce::AudioParameterInt* xParam;
    juce::AudioParameterInt* yParam;
    
    JIMath jim;
    
    int currentRefNote{};
    double currentRefFreq{};
    
    enum Direction
    {
        West,
        East,
        North,
        South,
        Home
    };
    
    void returnToOrigin();
    
    void respondToMidi(const juce::MidiMessage &m);
    void shift(int dir);
    void locate();
    
    void updateTuning();
    
    inline float GNV(int input);
    

    
    double ratios[12] = {};
    double freqs[128]{};
    
    double pyth12[12]
    {
        1.0,
        (double)256/243,
        (double)9/8,
        (double)32/27,
        (double)81/64,
        (double)4/3,
        (double)729/512,
        (double)3/2,
        (double)128/81,
        (double)27/16,
        (double)16/9,
        (double)243/128,
    };
    int pythCo[12]
    {
        0,
        -5,
        2,
        -3,
        4,
        -1,
        6,
        1,
        -4,
        3,
        -2,
        5
    };
    double duo12[12]
    {
        1.0,
        (double)16/15,
        (double)9/8,
        (double)6/5,
        (double)5/4,
        (double)4/3,
        (double)45/32,
        (double)3/2,
        (double)8/5,
        (double)5/3,
        (double)9/5,
        (double)15/8
    };
    std::pair<int, int> duoCo[12]
    {
        {0, 0},
        {-1, -1},
        {2, 0},
        {1, -1},
        {0, 1},
        {-1, 0},
        {2, 1},
        {1, 0},
        {0, -1},
        {-1, 1},
        {2, -1},
        {1, 1}
    };
    

    bool hold[5] = {false, false, false, false, false};
    bool wait[5] = {false, false, false, false, false};
    
//    juce::AudioProcessorValueTreeState state;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LatticesProcessor)
};
