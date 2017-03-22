/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
DelayAudioProcessor::DelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
        delayLine(2, 1)
#endif
{
    //initialize params
    this->delayTime = 500; //ms
    this->feedBack = 50; // %
    this->wetDry = 50; // %
    
    this->cook(delayTime, feedBack, wetDry);
    
    this->delayInSamples = 0;
    
    //initialize buffer
    this->delayLineLength = 1;
    this->readPos = 0;
    this->writePos = 0;
}

DelayAudioProcessor::~DelayAudioProcessor()
{
}

//==============================================================================
const String DelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double DelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayAudioProcessor::setCurrentProgram (int index)
{
}

const String DelayAudioProcessor::getProgramName (int index)
{
    return String();
}

void DelayAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // you have sample rate -- do the delayline length
    this->delayLineLength = (int)(2 * sampleRate);
    
    // sanity check to make sure the length isn't 0
    if(this->delayLineLength < 1)
    {
        this->delayLineLength = 1;
    }
    
    // initialize and clear delayline with this new value
    delayLine.setSize(2, this->delayLineLength);
    
    this->flush();
    
    this->cook(this->delayTime, this->feedBack, this->wetDry);

    
}

void DelayAudioProcessor::getInitial(double& delayTime_, double& feedBack_, double& wetDry_)
{
    // the public params
    delayTime_ = this->delayTime;
    feedBack_ = this->feedBack;
    wetDry_ = this->wetDry;
}

void DelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void DelayAudioProcessor::cook(double _delay, double _fdbck, double _wetdry)
{
    // twice the sample rate
    this->delayInSamples = _delay * (getSampleRate() / 1000);
    this->feedBack_norm = _fdbck / 100;
    this->wetDry_norm = _wetdry / 100;
    
    
    
    // set delay read position -- write remains at 0 until processing
    //this->readPos = (int)(this->writePos - delayInSamples + this->delayLineLength) % this->delayLineLength;
    this->readPos = writePos - (int)this->delayInSamples;
    
    if(this->readPos < 0)
    {
        this->readPos += this->delayLineLength; // wrap around... i like mod better
    }
};

void DelayAudioProcessor::flush()
{
    //reset delay!
    this->readPos = 0;
    this->writePos = 0;

    delayLine.clear();
}

void DelayAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    

    const int numSamples = buffer.getNumSamples();
    
    int readTmp;
    int readTmp_1;
    int writeTmp;
    
    
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        // write pointer to output buffer
        float* outputBuffer = buffer.getWritePointer (channel);
        // write pointer to delayline
        float* pDelayLine = this->delayLine.getWritePointer(channel);
        
        // temp copies of read/write indices
        readTmp = this->readPos;
        writeTmp = this->writePos;
        
        for(int sample = 0; sample < numSamples; sample++)
        {
            // if it's out of the circular buffer, wrap around
            readTmp_1 = (readTmp - 1) >= 0 ? readTmp - 1 : delayLineLength - 1;
            
            // read there
            float yn_1 = pDelayLine[readTmp_1];
            
            // percentage of length for lerp
            float u = this->delayInSamples - static_cast<int>(this->delayInSamples);
            
            // read output from delay line
            float yn = (pDelayLine[readTmp] * (1 - u)) + (yn_1 * u);
            
            // write input to delay line
            pDelayLine[writeTmp] = outputBuffer[sample] + (this->feedBack_norm)*yn;
            
            // update bookkeepers
            
            writeTmp++; // rewrite w mod
            if(writeTmp >= delayLineLength)
            {
                writeTmp = 0;
            }
            
            readTmp++;
            if(readTmp >= delayLineLength)
            {
                readTmp = 0;
            }
            
            
            // update output buffer
            outputBuffer[sample] = this->wetDry_norm * yn + (1.0 - this->wetDry_norm)*outputBuffer[sample];
            
        }

    }
    
    // update global read write pos
    this->readPos = readTmp;
    this->writePos = writeTmp;
}

//==============================================================================
bool DelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DelayAudioProcessor::createEditor()
{
    return new DelayAudioProcessorEditor (*this);
}

//==============================================================================
void DelayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayAudioProcessor();
}
