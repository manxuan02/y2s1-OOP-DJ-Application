#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Tracks.h"
#include <vector>
#include <string>

//==============================================================================
/*
*/
class PlaylistComponent : public juce::Component,
                          public TableListBoxModel,
                          public Button::Listener,
                          public TextEditor::Listener,
                          public DragAndDropContainer
{
public:
    PlaylistComponent();
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    // To implement 'PlaylistComponent' class
    int getNumRows() override;
    
    void paintRowBackground(Graphics & g,
                            int rowNumber,
                            int width,
                            int height,
                            bool rowIsSelected) override;
    
    void paintCell(Graphics & g,
                   int rowNumber,
                   int columnId,
                   int width,
                   int height,
                   bool rowIsSelected) override;
    
    // To create the 'Play' button
    Component* refreshComponentForCell(int rowNumber,
                                       int columnId,
                                       bool isRowSelected,
                                       Component *existingComponentToUpdate) override;
                                           
    // To implement the Button::Listener
    void buttonClicked(Button * button) override;
    
    // To add selected tracks from DeckGUI to tracklist
    void addTrack(File selectedFile);
    
    // To allow selected track (from the tracklist) to be loaded into WaveformDisplay
    void cellClicked(int rowNumber,
                     int columnId,
                     const MouseEvent&) override;
    
    // To allow for callback
    std::function<void(File)> onTrackPlay;
    
    void textEditorTextChanged(TextEditor& editor) override;
    
    void searchLibrary(const String& searchText);
    
private:
    // To create a Table, Table's Component
    TableListBox tableComponent;
    
    // To create a search bar
    TextEditor searchBox;
    
    // To use as a single pointer to the Entire_Tracks vector
    // points dynamically to Tracks
    std::vector<std::string> trackTitles;
    
    // To filter tracks in the search bar
    std::vector<std::string> filteredTrackTitles;
    
    // To store track files
    std::vector<File> trackFiles;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
