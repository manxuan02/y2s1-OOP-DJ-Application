#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager) 
    : formatManager(_formatManager), // Initialise 'formatManager'
    transportSource(),
    resampleSource(&transportSource, false),
    lowFilterSource(&transportSource, false),
    midFilterSource(&lowFilterSource, false),
    highFilterSource(&midFilterSource, false)
{

}

DJAudioPlayer::~DJAudioPlayer()
{

}

void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate) 
{
    // To get 'transportSource' Ready
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    // To get 'resampleSource' Ready to allow Variable_audio_Speed
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    // To set initial EQ filters
    lowFilter.setCoefficients(IIRCoefficients::makeLowShelf(sampleRate, 200.0, 0.707, lowGain));
    midFilter.setCoefficients(IIRCoefficients::makePeakFilter(sampleRate, 1000.0, 1.0, midGain));
    highFilter.setCoefficients(IIRCoefficients::makeHighShelf(sampleRate, 5000.0, 0.707, highGain));
    
    // To prepare the filter sources
    lowFilterSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    midFilterSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    highFilterSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // To get it to Run-Repeatedly
    resampleSource.getNextAudioBlock(bufferToFill);
    
    // To apply filter to modify the audio frequencies
    lowFilter.processSamples(bufferToFill.buffer->getWritePointer(0), bufferToFill.numSamples);
    midFilter.processSamples(bufferToFill.buffer->getWritePointer(0), bufferToFill.numSamples);
    highFilter.processSamples(bufferToFill.buffer->getWritePointer(0), bufferToFill.numSamples);
}

void DJAudioPlayer::releaseResources()
{
    // To release the 'AudioSource'
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

// To Load_the_File chosen
void DJAudioPlayer::loadURL(URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file!
    {       
        std::unique_ptr<AudioFormatReaderSource> newSource (new AudioFormatReaderSource (reader, true));
        transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);             
        readerSource.reset (newSource.release());          
    }
}

// To allow Variable_Volume
void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        // To prevent users from sending something out-of-range
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
    }
    else
    {
        // To push the value_obtained (gain), to 'transportSource'
        transportSource.setGain(gain);
    }
}

// To allow Variable_Speed
void DJAudioPlayer::setSpeed(double ratio)
{
  if (ratio < 0 || ratio > 10.0)
    {
        // To prevent users from sending something out-of-range
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 100" << std::endl;
    }
    else
    {
        // To allow Variable_Speed -- Putting 'resampleSource' into use
        resampleSource.setResamplingRatio(ratio);
    }
}

// To get the Position_of_the_audio_Wanted??
void DJAudioPlayer::setPosition(double posInSecs)
{
    // To receive the Playback_Position in seconds
    transportSource.setPosition(posInSecs);
}

// To allow changing of where_you_want_to_play the song at
void DJAudioPlayer::setPositionRelative(double pos)
{
     if (pos < 0 || pos > 1.0)
    {
        // To prevent users from sending something out-of-range
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
    }
    else
    {
        // To multiply [number coming_in] BY [length of the whole_audiofile]
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        
        // Send the value back
        setPosition(posInSecs);
    }
}

// To implement the Start & Stop functions
void DJAudioPlayer::start()
{
    transportSource.start();
    std::cout << "DJAudioPlayer::start() called for " << transportSource.getCurrentPosition() << std::endl;

}
void DJAudioPlayer::stop()
{
    transportSource.stop();
}

// To get Relative_Position of the Audio >> to draw the Moveable_Playhead
double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

// To implement and update EQ gains
void DJAudioPlayer::setEQGain(float low, float mid, float high)
{
    lowGain = low;
    midGain = mid;
    highGain = high;
    
    // To update filter coefficients
    lowFilter.setCoefficients(IIRCoefficients::makeLowShelf(44100, 200.0, 0.707, lowGain));
    midFilter.setCoefficients(IIRCoefficients::makeLowShelf(44100, 1000.0, 1.0, midGain));
    highFilter.setCoefficients(IIRCoefficients::makeLowShelf(44100, 5000.0, 0.707, highGain));
}

