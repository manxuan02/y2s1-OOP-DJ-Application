/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }  
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }  

    addAndMakeVisible(deckGUI1); 
    addAndMakeVisible(deckGUI2);
    
    addAndMakeVisible(playlistComponent);

    //NEED TO CALL THIS, to allow the program to know about the Audio_Formats used
    // To allow the program to READ THE AUDIO_FILE
    // To allow the program to RUN!!
    formatManager.registerBasicFormats();
    
    // To link DeckGUI to PlaylistComponent
    deckGUI1.onTrackSelected = [this](File file) {playlistComponent.addTrack(file);};
    deckGUI2.onTrackSelected = [this](File file) {playlistComponent.addTrack(file);};
    
    playlistComponent.onTrackPlay = nullptr;
//    // To set up PlaylistComponent to send selected tracks to DeckGUI
//    playlistComponent.onTrackPlay = [this](File file)
//    {
//        std::cout << "🎵 Sending track to DeckGUI: " << file.getFileName() << std::endl;
//        
//        // To load into the currently active deck
//        deckGUI1.loadTrackIntoWaveform(file);
//    };
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // To run DJAudioPlayer
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    // Setting up the Mixer_Configuration
//    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
}
void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // To get_the_audio from the Mixer
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // To show the Playlist UI Component
    g.setColour(Colours::white);
    g.setFont(14.0f);
    g.drawText("PlaylistComponent", getLocalBounds(), Justification::centred, true);
}

// To set the size of the components
void MainComponent::resized()
{
    deckGUI1.setBounds(0, 0, getWidth()/2, getHeight()/3 * 2);
    deckGUI2.setBounds(getWidth()/2, 0, getWidth()/2, getHeight()/3 * 2);
    
    playlistComponent.setBounds(0, getHeight()/3 * 2, getWidth(), getHeight()/2);
}
