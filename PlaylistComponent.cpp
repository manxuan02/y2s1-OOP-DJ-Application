#include "../JuceLibraryCode/JuceHeader.h"
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent() // Constructor To load Audio_file data
{
    // To access the folder that contains the tracks
    juce::File musicFolder("../../Source/tracks");

    // Iterate through files in the directory
    DirectoryIterator iterator(musicFolder, false, "*.*", File::findFiles);
    while(iterator.next()) {
        File fileFound = iterator.getFile();
        // Check if the file is a valid audio file
        if(fileFound.hasFileExtension("mp3") || 
            fileFound.hasFileExtension("wav"))
        {
            trackTitles.push_back(fileFound.getFileNameWithoutExtension().toStdString());
        }
    }
    
    // To add Columns
    tableComponent.getHeader().addColumn("Track Title (NEED TO DRAG TO THE PLAYLIST YOU WANT)", 1, 600);
    tableComponent.getHeader().addColumn("Play", 2, 200);
    tableComponent.getHeader().addColumn("Delete", 3, 200);
    tableComponent.setColour(juce::TableListBox::backgroundColourId, juce::Colours::powderblue);
    
    // To register as a Model
    tableComponent.setModel(this);
    
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(searchBox);
    
    searchBox.addListener(this);
    searchBox.setTextToShowWhenEmpty("Search tracks...", Colours::grey);
    searchBox.setColour(juce::TextEditor::backgroundColourId, juce::Colours::pink);
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint(Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId)); // to clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1); // to draw an outline around the component

    g.setColour (juce::Colours::chartreuse);
    g.setFont (juce::FontOptions (14.0f));
}

void PlaylistComponent::resized()
{
    tableComponent.setBounds(0, 0, getWidth(), getHeight());
    
    searchBox.setBounds(0, getHeight()/4 * 2.2, getWidth(), 30);
}

// To implement 'PlaylistComponent' class
int PlaylistComponent::getNumRows()
{
//    return 3; //returns 3 rows
    return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(Graphics & g, // To represent the Row that will be drawn in
                                           int rowNumber,
                                           int width,
                                           int height,
                                           bool rowIsSelected)
{
    // To just Highlight selected rows
    if(rowIsSelected)
    {
        g.fillAll(Colours::palegreen);
    }
    else
    {
        g.fillAll(Colours::honeydew);
    }
}

void PlaylistComponent::paintCell(Graphics& g, // To represent the Row that will be drawn in
                                  int rowNumber,
                                  int columnId,
                                  int width,
                                  int height,
                                  bool rowIsSelected)
{
    if (columnId == 1 && rowNumber < trackTitles.size())
    {
        g.drawText(trackTitles[rowNumber], //changeable
                       2,
                       0,
                       width - 4,
                       height,
                       Justification::centredLeft,
                       true);
    }
}

// To create the 'Play' buttons
Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
                                                      int columnId,
                                                      bool isRowSelected,
                                                      Component *existingComponentToUpdate) // A "pointer" to component
{
    if(columnId == 2)
    {
        if(existingComponentToUpdate == nullptr) // When a component is not created yet
        {
            TextButton* btn = new TextButton{"load"};
            btn->addListener(this);
            String id{std::to_string(rowNumber)};
            btn->setComponentID(id); //To put unique on the 'button'
            
            existingComponentToUpdate=btn;
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button* button)
{
    auto chooser = std::make_unique<FileChooser>(
        "Select an audio file",
        File(),
        "*.mp3;*.wav;"
    );

    chooser->launchAsync(FileBrowserComponent::canSelectFiles,
        [this](const FileChooser& fc)
        {
            File selectedFile = fc.getResult();
            std::cout << "FileChooser opened!" << std::endl;

            if (selectedFile.existsAsFile())
            {
                std::cout << "File selected: " << selectedFile.getFullPathName() << std::endl;

                // To add the file name to the tracklist
                trackTitles.push_back(selectedFile.getFileNameWithoutExtension().toStdString());

                // To confirm the new track is in the list
                std::cout << "Total tracks in list: " << trackTitles.size() << std::endl;

                // To refresh UI
                tableComponent.updateContent();
                repaint();
            }
            else
            {
                std::cout << "No valid file selected!" << std::endl;
            }
        }
    );
}

// To add selected tracks from DeckGUI to tracklist
void PlaylistComponent::addTrack(File selectedFile)
{
    std::cout << "🎵 Adding track to playlist: " << selectedFile.getFileNameWithoutExtension().toStdString() << std::endl;

    // To save the file for Loading later
    trackTitles.push_back(selectedFile.getFileNameWithoutExtension().toStdString());
    trackFiles.push_back(selectedFile);
    
    // To refresh UI
    tableComponent.updateContent();
    repaint();
}

// To allow selected track (from the tracklist) to be loaded into WaveformDisplay
void PlaylistComponent::cellClicked(int rowNumber,
                                    int columnId,
                                    const MouseEvent&)
{
    if (rowNumber < trackFiles.size())
    {
        std::cout << "Starting drag for: " << trackFiles[rowNumber].getFullPathName() << std::endl;
        
        // To create a StringArray & add the file path
        StringArray files; //because "performExternalDragDropOfFiles" expects a StringArray
        files.add(trackFiles[rowNumber].getFullPathName());
        
        // To allow dragging of the track file
        performExternalDragDropOfFiles(files, true);
    }
}

// To implement search box
void PlaylistComponent::textEditorTextChanged(TextEditor& editor)
{
    searchLibrary(editor.getText());
}

void PlaylistComponent::searchLibrary(const String& searchText)
{
    filteredTrackTitles.clear();
    for (const auto& title : trackTitles)
    {
        if (String(title).containsIgnoreCase(searchText))
        {
            filteredTrackTitles.push_back(title);
        }
    }
    tableComponent.updateContent();
}
