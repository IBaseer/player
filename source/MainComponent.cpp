//MainComponent.cpp
#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize(480, 120);
    addAndMakeVisible(playerGui);
}

MainComponent::~MainComponent() {}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void MainComponent::resized()
{
    playerGui.setBounds(getLocalBounds());
}
