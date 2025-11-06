#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"

class WaveformDisplay : public juce::Component, public juce::Timer, public ThemeManager::Listener
{
public:
	WaveformDisplay(PlayerAudio& player) : playerAudio(player)
	{
		startTimerHz(30);
		ThemeManager::getInstance().addListener(this);
	}

	~WaveformDisplay() override
	{
		ThemeManager::getInstance().removeListener(this);
	}

	void themeChanged() override
	{
		repaint();
	}

	void paint(juce::Graphics& g) override
	{
		auto& colors = ThemeManager::getInstance().getColors();

		g.fillAll(colors.background);

		auto bounds = getLocalBounds().reduced(2);
		g.setColour(colors.secondaryBackground);
		g.fillRect(bounds);

		juce::AudioThumbnail* thumbnail = playerAudio.getThumbnail();
		if (thumbnail != nullptr && thumbnail->getTotalLength() > 0.0)
		{
			g.setColour(colors.waveform);
			thumbnail->drawChannel(g, bounds, 0.0, thumbnail->getTotalLength(), 0, 0.8f);

			if (thumbnail->getNumChannels() > 1)
			{
				thumbnail->drawChannel(g, bounds, 0.0, thumbnail->getTotalLength(), 1, 0.8f);
			}

			double position = playerAudio.getCurrentPosition();
			double length = playerAudio.getLengthInSeconds();
			if (length > 0)
			{
				int xPos = bounds.getX() + (int)((position / length) * bounds.getWidth());
				g.setColour(colors.text);
				g.drawLine((float)xPos, (float)bounds.getY(), (float)xPos, (float)bounds.getBottom(), 2.0f);
			}
		}

		g.setColour(colors.border);
		g.drawRect(bounds, 1);
	}

	void timerCallback() override
	{
		repaint();
	}

private:
	PlayerAudio& playerAudio;
};

class PlayerGUI : public juce::Component,
	public juce::Button::Listener,
	public juce::Slider::Listener,
	public juce::Timer,
	public ThemeManager::Listener
{
public:
	PlayerGUI(const juce::String& playerName);
	~PlayerGUI() override;

	void resized() override;
	void paint(juce::Graphics& g) override;
	void themeChanged() override;

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
	void releaseResources();

	bool loadFile(const juce::File& file);
	PlayerAudio& getPlayerAudio() { return playerAudio; }

private:
	juce::String name;
	PlayerAudio playerAudio;
	WaveformDisplay waveformDisplay;

	juce::TextButton playButton{ "Play" };
	juce::TextButton pauseButton{ "Pause" };
	juce::TextButton restartButton{ "Restart" };
	juce::TextButton stopButton{ "Stop" };
	juce::TextButton muteButton{ "Mute" };
	juce::TextButton loopButton{ "Loop" };
	juce::TextButton setAButton{ "Set A" };
	juce::TextButton setBButton{ "Set B" };
	juce::TextButton clearLoopButton{ "Clear Loop" };
	juce::TextButton back10sButton{ "-10s" };
	juce::TextButton forward10sButton{ "+10s" };

	double loopStart = 0.0;
	double loopEnd = 0.0;

	juce::Slider volumeSlider;
	juce::Slider positionSlider;
	juce::Slider speedSlider;

	juce::Label nameLabel;
	juce::Label timeLabel;
	juce::Label fileNameLabel;
	juce::Label metadataLabel;
	juce::Label volumeLabel;
	juce::Label speedLabel;

	bool muted = false;
	float currentVolume = 0.7f;

	void buttonClicked(juce::Button* button) override;
	void sliderValueChanged(juce::Slider* slider) override;
	void timerCallback() override;
	juce::String formatTime(double seconds);
	void styleButton(juce::TextButton& button, juce::Colour colour);
	void applyThemeToComponents();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI);
};