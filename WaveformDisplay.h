/*
  ==============================================================================

    WaveformDisplay.h
    Created: 14 Mar 2020 3:50:16pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class WaveformDisplay : public Component,
                        public ChangeListener
{
public:
    WaveformDisplay( AudioFormatManager & formatManagerToUse, // Constructor To load Audio_Wave data
                    AudioThumbnailCache & cacheToUse ); // Constructor To load Audio_Wave data into "DeckGUI"
    ~WaveformDisplay();

    void paint (Graphics&) override;
    void resized() override;

    /** implement ChangeListener */
    void changeListenerCallback (ChangeBroadcaster *source) override;

    // To allow Loading of the File_Selected into WaveFromDisplay > To create Audio_Wave
    void loadURL(URL audioURL);

    /** set the relative position of the playhead*/
    void setPositionRelative(double pos);
    // relative = It will be between 0 and 1

private:
    AudioThumbnail audioThumb;
    
    // To record 'fileLoaded' state
    bool fileLoaded;
    
    // To store the Relative Position (received from 'DJAudioPlayer') for the Moveable_Playhead
    double position;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
