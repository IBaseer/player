#include "PlayerGui.h"

PlayerGui::PlayerGui()
{
    addAndMakeVisible(playButton);
    addAndMakeVisible(pauseButton);
    addAndMakeVisible(startButton);
    addAndMakeVisible(endButton);

    playButton.addListener(this);
    pauseButton.addListener(this);
    startButton.addListener(this);
    endButton.addListener(this);
}

PlayerGui::~PlayerGui() {}

void PlayerGui::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void PlayerGui::resized()
{
    auto area = getLocalBounds().reduced(10);
    auto width = 100;
    auto height = 40;
    int y = 10;

    playButton.setBounds(10, y, width, height);
    pauseButton.setBounds(120, y, width, height);
    startButton.setBounds(230, y, width, height);
    endButton.setBounds(340, y, width, height);
}

void PlayerGui::buttonClicked(juce::Button* b)
{
    if (b == &playButton) player.play();
    else if (b == &pauseButton) player.pause();
    else if (b == &startButton) player.goToStart();
    else if (b == &endButton) player.goToEnd();
}
