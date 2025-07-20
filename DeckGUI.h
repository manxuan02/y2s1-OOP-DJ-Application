#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "CustomLookAndFeel.h"

//==============================================================================
/*
*/
class DeckGUI    : public Component,
                   public Button::Listener, 
                   public Slider::Listener, 
                   public FileDragAndDropTarget,
                   public DragAndDropTarget,
                   public Timer //To allow the Moveable_Playhead to MOVE along w the song
{
public:
    DeckGUI(DJAudioPlayer* player1,
            DJAudioPlayer* player2,
            bool isPlayer1,
           AudioFormatManager & formatManagerToUse, // To load Audio_Wave data into "DeckGUI"
           AudioThumbnailCache & cacheToUse ); // To load Audio_Wave data into "DeckGUI"
    ~DeckGUI();

    void paint (Graphics&) override;
    void resized() override;

     /** implement Button::Listener */
    void buttonClicked (Button *) override;

    /** implement Slider::Listener */
    void sliderValueChanged (Slider *slider) override;

    /** implement FileDragAndDropTarget */
    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override;

    /** implement DragAndDropTarget */
    bool isInterestedInDragSource (const SourceDetails& dragSourceDetails) override;
    void itemDropped (const SourceDetails& dragSourceDetails) override;
    
    /** implement Timer */
    void timerCallback() override;
    
    // To send selected track to PlaylistComponent
    std::function<void(File)> onTrackSelected;
    
    // To allow loading of tracks to WaveformComponent
    void loadTrackIntoWaveform(File file);

private:
    TextButton playButton{"PLAY"};
    TextButton stopButton{"STOP"};
    TextButton loadButton{"LOAD"};
  
    Slider volSlider{Slider::LinearVertical, // To place the volSlider Vertically
                     Slider::TextBoxBelow}; // To place the text box Below the volSlider
    Slider speedSlider;
    Slider posSlider;
    
    FileChooser fChooser{"Select a file..."};

    DJAudioPlayer* player; // For BOTH player
    DJAudioPlayer* player1; // For player1 oNLY
    DJAudioPlayer* player2; // For player2 oNLY
    
    // For equalizer sliders
    Slider lowEQSlider;
    Slider midEQSlider;
    Slider highEQSlider;
    
    bool isPlayer1Instance; // To check which GUI it belongs to, player1 || player2
    
    WaveformDisplay waveformDisplay;
    
    CustomLookAndFeel customLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
