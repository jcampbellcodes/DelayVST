/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
DelayAudioProcessorEditor::DelayAudioProcessorEditor (DelayAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (300, 300);
    setResizable(true, false);
    processor.getInitial(this->delayTime_ms, this->feedBack_pct, this->wetDry_pct);
    
    // delay time
    addAndMakeVisible(this->delaySlider);
    this->delaySlider.setSliderStyle(Slider::LinearBarVertical);
    this->delaySlider.setRange(10, 2000);
    this->delaySlider.setTextValueSuffix(" ms");
    this->delaySlider.addListener(this);
    this->delaySlider.setValue(delayTime_ms);
    
    // feedback
    addAndMakeVisible(this->feedBackSlider);
    this->feedBackSlider.setSliderStyle(Slider::LinearBarVertical);
    this->feedBackSlider.setRange(0, 100);
    this->feedBackSlider.setTextValueSuffix(" %");
    this->feedBackSlider.addListener(this);
    this->feedBackSlider.setValue(this->feedBack_pct);
    
    // wet-dry
    addAndMakeVisible(this->wetDrySlider);
    this->wetDrySlider.setSliderStyle(Slider::LinearBarVertical);
    this->wetDrySlider.setRange(0, 100);
    this->wetDrySlider.setTextValueSuffix(" %");
    this->wetDrySlider.addListener(this);
    this->wetDrySlider.setValue(wetDry_pct);
    
    addAndMakeVisible(this->delayLabel);
    this->delayLabel.setText("Delay Time", dontSendNotification);
    this->delayLabel.attachToComponent(&delaySlider, false);

    addAndMakeVisible(this->feedBackLabel);
    this->feedBackLabel.setText("Feedback", dontSendNotification);
    this->feedBackLabel.attachToComponent(&feedBackSlider, false);
    
    addAndMakeVisible(this->wetDryLabel);
    this->wetDryLabel.setText("Wet/Dry ", dontSendNotification);
    this->wetDryLabel.attachToComponent(&wetDrySlider, false);
    
}

DelayAudioProcessorEditor::~DelayAudioProcessorEditor()
{
}

//==============================================================================
void DelayAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::whitesmoke);
    g.setOpacity(0.1f);
    g.setColour (Colours::black);
    g.setFont(Font("Arial", 20.0f, Font::FontStyleFlags::bold | Font::FontStyleFlags::italic | Font::FontStyleFlags::italic));
    g.drawFittedText ("STUTTERRRRRRR", 0, 0, getWidth()/4, getHeight() / 15, Justification::top, 1);
}

void DelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    delaySlider.setBounds (0, getHeight()/5, getWidth() / 3, getHeight() - 100);
    feedBackSlider.setBounds (getWidth()/3, getHeight()/5, getWidth() / 3, getHeight() - 100);
    wetDrySlider.setBounds (getWidth() - (getWidth()/3), getHeight()/5, getWidth() / 3, getHeight() - 100);
}

void DelayAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    // update all values
    this->delayTime_ms = delaySlider.getValue();
    this->feedBack_pct = feedBackSlider.getValue();
    this->wetDry_pct = wetDrySlider.getValue();
    // refactor this...
    this->processor.cook(this->delayTime_ms, this->feedBack_pct, this->wetDry_pct);
    
}
