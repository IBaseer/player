#include "MainComponent.h"
#include "PlayerGUI.h" // التضمين هنا فقط

MainComponent::MainComponent()
{
    // إنشاء المشغلين
    player1 = std::make_unique<PlayerGUI>();
    player2 = std::make_unique<PlayerGUI>();

    addAndMakeVisible(player1.get());
    addAndMakeVisible(player2.get());

    // إضافة المشغلين إلى المازج
    mixerSource.addInputSource(player1.get(), false);
    mixerSource.addInputSource(player2.get(), false);

    setSize(1200, 600);
    setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
    mixerSource.removeAllInputs();
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    player1->prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2->prepareToPlay(samplesPerBlockExpected, sampleRate);
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    player1->releaseResources();
    player2->releaseResources();
    mixerSource.releaseResources();
}

void MainComponent::resized()
{
    auto area = getLocalBounds();

    // تقسيم المساحة إلى نصفين أفقيين
    player1->setBounds(area.removeFromLeft(getWidth() / 2));
    player2->setBounds(area);
}