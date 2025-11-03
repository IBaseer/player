#pragma once

#include <JuceHeader.h>

class PlayerAudio : public juce::AudioSource
{
public:
    PlayerAudio();
    ~PlayerAudio() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    bool loadFile(const juce::File& file);
    bool loadFileFromPlaylist(const juce::File& file);
    void start();
    void stop();
    void setGain(float gain);
    void setPosition(double pos);
    double getPosition() const;
    double getLength() const;

    void setMute(bool shouldMute);
    bool getMuted() const { return muted; }

    void setLooping(bool shouldLoop);
    bool getLooping() const { return isLooping; }

    juce::String getCurrentFileName() const;
    juce::String getMetadata() const;
    juce::String getTitle() const;
    juce::String getArtist() const;
    double getDuration() const;

    bool hasFileLoaded() const { return readerSource != nullptr; }
    bool isPlaying() const { return transportSource.isPlaying(); }
    float getGain() const { return transportSource.getGain(); }

private:
    bool muted = false;
    bool isLooping = false;
    float previousGain = 1.0f;
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;

    juce::File currentFile;
    juce::String title;
    juce::String artist;
    double duration = 0.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};