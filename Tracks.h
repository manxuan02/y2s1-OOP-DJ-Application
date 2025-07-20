#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <string>

struct Tracks
{
    // Store track's title as a String
    String title;
    
    // Length of the track in seconds
    double lengthInSec;
    
    // Url of the track (file path of the track)
    URL url;
    
    // To Represent the track
    String identity;
    
    // To Convert the track's length from seconds > string
    static std::string getLengthAsString(double songLength, //is in Seconds
                                         bool regUpdate = false)
    {
        // Track's length as String
        std::string time = "";
        
        // Extract Track's length >> Hour
        int hour = std::floor(songLength / 3600);
        // Extract Track's length >> Minute
        int min = std::floor((songLength - ((double)hour * 3600)) / 60);
        /*
         hour * 3600: converts hour >> seconds
         songLength -: to Leave only the *Remaining Seconds*
         / 60: converts *Remaining Seconds* >> minutes
         */
        
        // Extract Track's length >> Seconds
        double sec = std::floor((songLength - ((double)min * 60)) * 100) / 100;
        /*
         min * 60: converts minute >> seconds
         songLength -: to Leave only the *Remaining Seconds*
         * 100: to handle floating-point for seconds
         / 100: to Preserve only 2 dp
         */
        
        // Extract Track's length >> MilliSeconds
        double milliSec = std::floor((sec - std::floor(sec)) * 100);
        /*
         sec - std::floor(sec): to get the decimal part, for seconds
         * 100: to handle floating-point
         */
        
        if(!regUpdate)
        {
            if(hour < 10)
            {
                // To produce "01:" instead of "1:"
                time += "0" + std::to_string(hour) + ":";
            }
            else
            {
                time += std::to_string(hour) + ":";
            }
        }

        if(min < 10)
        {
            time += "0" + std::to_string(min) + ":";
        }
        else
        {
            time += std::to_string(min) + ":";
        }

        if(sec < 10)
        {
            time += "0" + std::to_string((int)sec);
        }
        else
        {
            time += std::to_string((int)sec);
        }

        if(regUpdate)
        {
            if(milliSec < 10)
            {
                time += ":0" + std::to_string(milliSec);
            }
            else
            {
                time += ":" + std::to_string(milliSec);
            }
        }
        
        return time;
    };
};
