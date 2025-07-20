/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
#include "CustomLookAndFeel.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //For AudioSource Interface
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //For showing the GUI
    void paint (Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
     
    // To implement the Audio_Wave
    AudioFormatManager formatManager;
    AudioThumbnailCache thumbCache{100};
    //{100} - Means that it can store upto 100cached_waveforms = load_100_files

    // To create/link/call DJAudioPlayer
    DJAudioPlayer player1{formatManager}; // Pass in 'formatManager'
    // To create/link/call DeckGUI
    DeckGUI deckGUI1{&player1, &player2, true, formatManager, thumbCache};

    // To create/link/call DJAudioPlayer
    DJAudioPlayer player2{formatManager}; // Pass in 'formatManager'
    // To create/link/call DeckGUI
    DeckGUI deckGUI2{&player2, &player1, false, formatManager, thumbCache};

    // To play 2 files at once
    MixerAudioSource mixerSource;
    
    // To create/link/call PlaylistComponent
    PlaylistComponent playlistComponent;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
