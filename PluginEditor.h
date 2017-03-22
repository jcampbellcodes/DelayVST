/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class DelayAudioProcessorEditor  : public AudioProcessorEditor,
Slider::Listener
{
public:
    DelayAudioProcessorEditor (DelayAudioProcessor&);
    ~DelayAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void sliderValueChanged(Slider* slider) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    
    double delayTime_ms;
    double feedBack_pct;
    double wetDry_pct;
    
    Slider delaySlider;
    Label delayLabel;
    Slider feedBackSlider;
    Label feedBackLabel;
    Slider wetDrySlider;
    Label wetDryLabel;
    
    DelayAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
