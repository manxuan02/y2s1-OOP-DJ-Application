/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 13 Mar 2020 4:22:22pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public AudioSource {
  public:

    DJAudioPlayer(AudioFormatManager& _formatManager); // so it will receive 'AudioFormatManager'
    ~DJAudioPlayer();

    //For AudioSource Interface
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //Functions that wil be used
    void loadURL(URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);
    
    void start();
    void stop();

    /** To get the _Relative Position_ of the Moveable_Playhead*/
    double getPositionRelative();
    // relative = It will be between 0 n 1
    
    // To implement the equalizer
    void setEQGain(float low, float mid, float high);

private:
    // To supply the informations - Audio format_Manager
    AudioFormatManager& formatManager;
    
    // A smart_pointer, To allow the program to Run the Selected File - Audio format_ReaderSource needs a unique pointer
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    
    // A transport_layer, To have Better Playback_Control - Audio format_TransportSource
    AudioTransportSource transportSource;
    
    // To allow to have Variable_Speed
    ResamplingAudioSource resampleSource{&transportSource, false};
    //reference pointer, not to change the pointer, 2 channels
    
    // To implement the equalizer
    IIRFilter lowFilter, midFilter, highFilter;
    IIRFilterAudioSource lowFilterSource{&transportSource, false};
    IIRFilterAudioSource midFilterSource{&transportSource, false};
    IIRFilterAudioSource highFilterSource{&transportSource, false};
    
    // For EQ Gain values
    float lowGain = 1.0f;
    float midGain = 1.0f;
    float highGain = 1.0f;
};




