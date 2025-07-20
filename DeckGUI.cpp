#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player1,
                 DJAudioPlayer* _player2,
                 bool isPlayer1,
                 AudioFormatManager & formatManagerToUse, // Constructor To load Audio_Wave data into "DeckGUI"
                 AudioThumbnailCache & cacheToUse //Constructor To load Audio_Wave data into "DeckGUI"
           ) : player1(_player1),
               player2(_player2),
               isPlayer1Instance(isPlayer1),
               waveformDisplay(formatManagerToUse, cacheToUse)
{
    // To assign the GUI to the correct player
    player = isPlayer1 ? player1 : player2;
    
    // To Display them
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
       
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    
    addAndMakeVisible(lowEQSlider);
    addAndMakeVisible(midEQSlider);
    addAndMakeVisible(highEQSlider);

    addAndMakeVisible(waveformDisplay);

    // To add Listener
    playButton.addListener(this);
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::maroon);
    stopButton.addListener(this);
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::maroon);
    loadButton.addListener(this);
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkorange);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    // To set the Range
    volSlider.setRange(0.0, 1.0, 0.01);
    speedSlider.setRange(0.1, 3.0, 0.01);
    posSlider.setRange(0.0, 1.0, 0.01);
    
    lowEQSlider.setRange(0.5, 2.0, 0.01);
    midEQSlider.setRange(0.5, 2.0, 0.01);
    highEQSlider.setRange(0.5, 2.0, 0.01);
    
    lowEQSlider.onValueChange = [this] { player->setEQGain(lowEQSlider.getValue(), midEQSlider.getValue(), highEQSlider.getValue()); };
    midEQSlider.onValueChange = [this] { player->setEQGain(lowEQSlider.getValue(), midEQSlider.getValue(), highEQSlider.getValue()); };
    highEQSlider.onValueChange = [this] { player->setEQGain(lowEQSlider.getValue(), midEQSlider.getValue(), highEQSlider.getValue()); };
    
    // For Moveable_Playhead
    startTimer(500); //500 milliseconds = how often it will be Called Back
    
    // To implement the custom designs
    volSlider.setLookAndFeel(&customLookAndFeel);
    // To set the Initial Value
    volSlider.setValue(0.5);
    speedSlider.setValue(1);
    // Setting the layout
    volSlider.setSliderStyle(Slider::LinearVertical);
    volSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20); //newPosition, bool (true = is a Read-Only display), width, height
    
    lowEQSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    lowEQSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    lowEQSlider.setLookAndFeel(&customLookAndFeel);

    midEQSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    midEQSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    midEQSlider.setLookAndFeel(&customLookAndFeel);

    highEQSlider.setSliderStyle(Slider::RotaryHorizontalVerticalDrag);
    highEQSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    highEQSlider.setLookAndFeel(&customLookAndFeel);
}

DeckGUI::~DeckGUI()
{
    // For Moveable_Playhead,, To destroy it
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
//    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
    g.fillAll (juce::Colours::darkslategrey);   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::chartreuse);
    g.setFont (14.0f);
    g.drawText ("DeckGUI", getWidth() / 2, 0, getWidth()/5, getHeight()/9 , Justification::centred, true);   // draw some placeholder text
    
    // Draw labels
    g.drawText("volume", volSlider.getBounds(), Justification::topLeft, false);
    g.drawText("speed", speedSlider.getBounds(), Justification::topLeft, false);
    g.drawText("track position", posSlider.getBounds(), Justification::topLeft, false);
    
    // Draw Equalizer Labels
    g.drawText("Low EQ", lowEQSlider.getBounds(), Justification::centredBottom, false);
    g.drawText("Mid EQ", midEQSlider.getBounds(), Justification::centredBottom, false);
    g.drawText("High EQ", highEQSlider.getBounds(), Justification::centredBottom, false);
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 9;
    
    //x, y, width, height
    waveformDisplay.setBounds(0, 0, getWidth(), rowH * 2);
    
    playButton.setBounds(0, rowH * 2, getWidth(), rowH);
    stopButton.setBounds(0, rowH * 3, getWidth(), rowH);
    
    if (isPlayer1Instance)
    {
        // To draw Player1, on the Left
        volSlider.setBounds(getWidth()/2 + 100, rowH * 4, 50, rowH * 4);
        speedSlider.setBounds(0, rowH * 4, getWidth()/2, rowH);
        posSlider.setBounds(0, rowH * 5, getWidth()/2, rowH);
        
        lowEQSlider.setBounds(getWidth() / 2 - 120, rowH * 6, 70, 70);
        midEQSlider.setBounds(getWidth() / 2 - 40, rowH * 6, 70, 70);
        highEQSlider.setBounds(getWidth() / 2 + 40, rowH * 6, 70, 70);
    }
    else
    {
        // To draw Player2, on the Right
        volSlider.setBounds(getWidth()/2 - 150, rowH * 4, 50, rowH * 4);
        speedSlider.setBounds(getWidth()/2 * 1, rowH * 4, getWidth()/2, rowH);
        posSlider.setBounds(getWidth()/2 * 1, rowH * 5, getWidth()/2, rowH);
        
        lowEQSlider.setBounds(getWidth() / 2 + 120, rowH * 6, 70, 70);
        midEQSlider.setBounds(getWidth() / 2 + 40, rowH * 6, 70, 70);
        highEQSlider.setBounds(getWidth() / 2 - 40, rowH * 6, 70, 70);
    }
    
    loadButton.setBounds(0, rowH * 8, getWidth(), rowH);
}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        player1->start();
        player2->start();
    }
    else if (button == &stopButton)
    {
        std::cout << "Stop button was clicked " << std::endl;
        player1->stop();
        player2->stop();
    }
    else if (button == &loadButton)
    {
       auto fileChooserFlags = 
        FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {
            File chosenFile = chooser.getResult();
            if (chosenFile.exists()){
                std::cout << "File chosen: " << chosenFile.getFullPathName() << std::endl;
                
                // To send the selected track to PlaylistComponent
                if (onTrackSelected)
                {
                    onTrackSelected(chosenFile);
                }
            }
        });
    }
}

void DeckGUI::sliderValueChanged (Slider *slider)
{
    // To allow changing of the song's volume
    if (slider == &volSlider)
    {
        // To run the 'slider - setGain' setting in 'DJAudioPlayer.cpp'
        player->setGain(slider->getValue());
    }

    // To allow changing of the song's speed
    if (slider == &speedSlider)
    {
        // To run the 'slider - setSpeed' setting in 'DJAudioPlayer.cpp'
        player->setSpeed(slider->getValue());
    }
    
    // To allow changing of where_you_want_to_play the song at
    if (slider == &posSlider)
    {
        // To run the 'slider - resampleSource' setting in 'DJAudioPlayer.cpp'
        player->setPositionRelative(slider->getValue());
    }
}

// To implement File_Dragging
bool DeckGUI::isInterestedInDragSource(const juce::DragAndDropTarget::SourceDetails& dragSourceDetails)
{
    std::cout << "DeckGUI isInterestedIn Drag" << dragSourceDetails.description.toString() << std::endl;
    return true;
}

void DeckGUI::itemDropped(const SourceDetails& dragSourceDetails)
{
    std::cout << "track dropped into DeckGUI" << std::endl;
    File droppedFile(dragSourceDetails.description.toString());
    
    if (droppedFile.exists())
    {
        std::cout << "Loading track into player: " << droppedFile.getFullPathName() << std::endl;
        
        if (isPlayer1Instance)
        {
            // To Load into the correct player and waveform
            player1->loadURL(URL{droppedFile});
            waveformDisplay.loadURL(URL{droppedFile});
        }
        else
        {
            // To Load into the correct player and waveform
            player2->loadURL(URL{droppedFile});
            waveformDisplay.loadURL(URL{droppedFile});
        }
    }
}
bool DeckGUI::isInterestedInFileDrag (const juce::StringArray &files)
{
    std::cout << "File drag detected!" << std::endl;
    return true; // Accept file drag
}

void DeckGUI::filesDropped (const juce::StringArray &files, int x, int y)
{
    if (files.size() > 0)
    {
        juce::File droppedFile(files[0]); // Take the first dropped file

        if (droppedFile.existsAsFile())
        {
            std::cout << "File dropped: " << droppedFile.getFullPathName() << std::endl;
            
            if (isPlayer1Instance)
            {
                // To Load into the correct player and waveform
                player1->loadURL(URL{droppedFile});
                waveformDisplay.loadURL(URL{droppedFile});
            }
            else
            {
                // To Load into the correct player and waveform
                player2->loadURL(URL{droppedFile});
                waveformDisplay.loadURL(URL{droppedFile});
            }
        }
    }
}

// To allow the Moveable_Playhead to MOVE together w the Audio
void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}

// To allow loading of tracks to WaveformComponent
void DeckGUI::loadTrackIntoWaveform(File file)
{
    std::cout << "Loading track into WaveformDisplay: " << file.getFullPathName() << std::endl;
    
    // To load file into WaveformComponent
    waveformDisplay.loadURL(URL{file});
}
