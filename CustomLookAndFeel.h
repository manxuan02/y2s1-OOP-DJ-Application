/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 14 Feb 2025 9:23:21am
    Author:  Em Ex

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Tracks.h"

//==============================================================================
/*
*/
class CustomLookAndFeel  : public Component,
                           public LookAndFeel_V4
{
public:
    CustomLookAndFeel();
    ~CustomLookAndFeel() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void drawRotarySlider (Graphics & g,
                           int x,
                           int y,
                           int width,
                           int height,
                           float sliderPosProportional,
                           float rotaryStartAngle,
                           float rotaryEndAngle,
                           Slider &) override;
    
    void drawLinearSlider (Graphics &,
                           int x,
                           int y,
                           int width,
                           int height,
                           float sliderPos,
                           float minSliderPos,
                           float maxSliderPos,
                           Slider::SliderStyle,
                           Slider &) override;

private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomLookAndFeel)
};
