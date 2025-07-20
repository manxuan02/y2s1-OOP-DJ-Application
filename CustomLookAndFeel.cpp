/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 14 Feb 2025 9:23:21am
    Author:  Em Ex

  ==============================================================================
*/

#include "CustomLookAndFeel.h"

//==============================================================================
CustomLookAndFeel::CustomLookAndFeel()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

CustomLookAndFeel::~CustomLookAndFeel()
{
}

void CustomLookAndFeel::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("CustomLookAndFeel", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void CustomLookAndFeel::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void CustomLookAndFeel::drawRotarySlider (Graphics & g,
                                          int x,
                                          int y,
                                          int width,
                                          int height,
                                          float sliderPosProportional,
                                          float rotaryStartAngle,
                                          float rotaryEndAngle,
                                          Slider &)
{
    auto radius = (float) juce::jmin (width / 2, height / 2) - 4.0f;
    auto centreX = (float) x + (float) width  * 0.5f;
    auto centreY = (float) y + (float) height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    
    // To fill the Dial
    g.setColour (Colours::orange);
    g.fillEllipse (rx, ry, rw, rw);

    // To draw the Outline
    g.setColour (Colours::red);
    g.drawEllipse (rx, ry, rw, rw, 2.0f);
    
    // To Translate & Rotate the position of the Dial by Angle
    juce::Path p;
    auto pointerLength = radius * 0.6f;
    auto pointerThickness = 3.0f;
    p.addRectangle (-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform (juce::AffineTransform::rotation (angle).translated (centreX, centreY));
    
    // To draw the POinter
    g.setColour (juce::Colours::yellow);
    g.fillPath (p);
}

void CustomLookAndFeel::drawLinearSlider (Graphics & g,
                                          int x,
                                          int y,
                                          int width,
                                          int height,
                                          float sliderPos,
                                          float minSliderPos,
                                          float maxSliderPos,
                                          Slider::SliderStyle,
                                          Slider &slider)
{
    // Background track
    g.setColour(Colours::darkgrey);
    g.fillRect(x + width / 2 - 2, y, 4, height); //x, y, width, height

    // Slider thumb
    g.setColour(Colours::white);
    g.fillEllipse(x + width / 2 - 5, sliderPos - 5, 10, 10); //x, y, width, height
}
