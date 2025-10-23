//PlayerGui.h
#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlayerGui : public juce::Component, public juce::Button::Listener
{
public:
    PlayerGui();
    ~PlayerGui() override;
    void paint(juce::Graphics&) override;
    void resized() override;
    void buttonClicked(juce::Button* button) override;

private:
    PlayerAudio player;
    juce::TextButton playButton{ "Play" };
    juce::TextButton pauseButton{ "Pause" };
    juce::TextButton startButton{ "Start" };
    juce::TextButton endButton{ "End" };


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGui)
};
