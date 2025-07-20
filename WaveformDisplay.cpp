/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 14 Mar 2020 3:50:16pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager & formatManagerToUse, // Constructor To load Audio_Wave data
                                 AudioThumbnailCache & cacheToUse) // Constructor To load Audio_Wave data into "DeckGUI"
                                : audioThumb(1000, formatManagerToUse, cacheToUse), //audio_resolution, audio_format, audio_cache
                                 fileLoaded(false),
                                 position(0) // Initialise the position (for Relative Position)
                          
{
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::orchid);
    
    //If file is Loaded >> audioThumb will Draw Audio_Wave
    if(fileLoaded)
    {
        audioThumb.drawChannel(g, //calls all the function in 'paint' to draw.. like a canvas
                               getLocalBounds(), //amount of space the graphics will take up
                               0, //time to Start drawing
                               audioThumb.getTotalLength(), //time to End drawing
                               0, //number of channels to draw
                               1.0f //vertical zoom factor
                               );
        
        // To draw the Moveable_Playhead
        g.setColour(Colours::lightgreen);
        g.drawRect(position * getWidth(), 0, getWidth() / 125, getHeight(), 5); //x, y, width, height
    }
    // If file is NotLoaded >> NTH will be drawn
    else
    {
        g.setFont (20.0f);
        g.drawText ("File not loaded...", getLocalBounds(), Justification::centred, true);   // draw some placeholder text
    }
}

void WaveformDisplay::resized()
{
}

// To allow Loading of the File_Selected into WaveFromDisplay
// - This function will be Called_by DeckGUI, by the Button::Listener
void WaveformDisplay::loadURL(URL audioURL)
{
    // To clear the old Audio_Wave
    audioThumb.clear();
    
    // To Pass URL to audioThumbnail
    fileLoaded  = audioThumb.setSource(new URLInputSource(audioURL));
    //takes the URL > turns it into an InputSource by_calling_'new URLInputSource' > calls 'setSource'
    
    if (fileLoaded)
    {
        std::cout << "wfd: loaded! " << std::endl;
        repaint();
    }
    else
    {
        std::cout << "wfd: not loaded! " << std::endl;
    }
}

//changeBroadcaster - To tell others when there's a Change
//changeListener - To lookout for Change in the Audio_Wave > then sends out the changes accordingly
void WaveformDisplay::changeListenerCallback (ChangeBroadcaster *source)
{
    std::cout << "wfd: change received! " << std::endl;

    repaint();
}

// To implement What_Happens_after 'WaveformDisplay' Received the 'relative position' ,, To draw the Moveable_Playhead
void WaveformDisplay::setPositionRelative(double pos)
{
    // To ONLY call 'repaint' >> IF 'position' is Updated
    if (pos != position)
    {
        // To store the Relative Position (received from 'DJAudioPlayer')
        position = pos;
        
        repaint();
    }
}
