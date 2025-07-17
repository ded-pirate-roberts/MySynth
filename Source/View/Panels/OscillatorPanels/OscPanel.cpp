/*
  ==============================================================================

    OscPanel.cpp
    Created: 24 Jun 2025 7:53:59pm
    Author:  Caleb Balboni

  ==============================================================================
*/
#include "OscPanel.h"

OscPanel::OscPanel(MySynthAudioProcessor& processor) : osc11(processor, "OSC11"),
                                                       osc12(processor, "OSC12"),
                                                       osc13(processor, "OSC13"),
                                                       osc21(processor, "OSC21"),
                                                       osc22(processor, "OSC22"),
                                                       osc23(processor, "OSC23"),
                                                       osc31(processor, "OSC31"),
                                                       osc32(processor, "OSC32"),
                                                       osc33(processor, "OSC33"),
                                                       adsr(processor, "MASTER") {
    // this window stuff
    panelName.setText("Oscillator controls:", juce::dontSendNotification);
    
    // FIELDS SETUP:
    
    
    
    
    // make everything visible
    addAndMakeVisible(panelName);
    
    addAndMakeVisible(&osc11);
    addAndMakeVisible(&osc12);
    addAndMakeVisible(&osc13);
    addAndMakeVisible(&osc21);
    addAndMakeVisible(&osc22);
    addAndMakeVisible(&osc23);
    addAndMakeVisible(&osc31);
    addAndMakeVisible(&osc32);
    addAndMakeVisible(&osc33);
    addAndMakeVisible(adsr);
}

OscPanel::~OscPanel() {
}

void OscPanel::resized()
{
    constexpr int yOffset = 25;      // shift everything down
    auto area       = getLocalBounds().reduced(10);
    auto topHalf    = area.removeFromTop(140);
    auto bottomHalf = area;

    // move ADSR sub-panel down
    adsr.setBounds(0, yOffset, 400, 150);

    // layout the 3×3 grid of SingleOscPanels, also shifted down
    constexpr int cols = 3, rows = 3, gridGap = 6;
    auto cellW = (bottomHalf.getWidth()  - (cols - 1) * gridGap) / cols;
    auto cellH = (bottomHalf.getHeight() - (rows - 1) * gridGap) / rows;

    auto place = [&bottomHalf,cellW,cellH,gridGap,yOffset](juce::Component& c, int col, int row)
    {
        int x = bottomHalf.getX() + col * (cellW + gridGap);
        int y = bottomHalf.getY() + row * (cellH + gridGap) + yOffset;
        c.setBounds(x, y, cellW, cellH);
    };

    place(osc11, 0, 0);  place(osc12, 1, 0);  place(osc13, 2, 0);
    place(osc21, 0, 1);  place(osc22, 1, 1);  place(osc23, 2, 1);
    place(osc31, 0, 2);  place(osc32, 1, 2);  place(osc33, 2, 2);
}
