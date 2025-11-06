#include "PlayerGui.h"

void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	playerAudio.getNextAudioBlock(bufferToFill);
}

void PlayerGUI::releaseResources()
{
	playerAudio.releaseResources();
}

void PlayerGUI::paint(juce::Graphics& g)
{
	auto& colors = ThemeManager::getInstance().getColors();

	g.fillAll(colors.secondaryBackground);

	auto bounds = getLocalBounds();
	g.setColour(colors.border);
	g.drawRect(bounds, 2);
}

PlayerGUI::PlayerGUI(const juce::String& playerName)
	: name(playerName), waveformDisplay(playerAudio)
{
	ThemeManager::getInstance().addListener(this);

	for (auto* btn : {
		&playButton, &pauseButton, &stopButton, &restartButton,&muteButton, &loopButton,
		&setAButton, &setBButton, &clearLoopButton,
		&back10sButton, &forward10sButton
		})
	{
		btn->addListener(this);
		addAndMakeVisible(btn);
	}

	volumeSlider.setRange(0.0, 1.0, 0.01);
	volumeSlider.setValue(0.7);
	volumeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
	volumeSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 40, 20);
	volumeSlider.addListener(this);
	addAndMakeVisible(volumeSlider);

	positionSlider.setRange(0.0, 1.0, 0.001);
	positionSlider.setSliderStyle(juce::Slider::LinearHorizontal);
	positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
	positionSlider.addListener(this);
	addAndMakeVisible(positionSlider);

	speedSlider.setRange(0.5, 2.0, 0.1);
	speedSlider.setValue(1.0);
	speedSlider.setSliderStyle(juce::Slider::LinearHorizontal);
	speedSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 40, 20);
	speedSlider.addListener(this);
	addAndMakeVisible(speedSlider);

	nameLabel.setText(name, juce::dontSendNotification);
	nameLabel.setJustificationType(juce::Justification::centred);
	nameLabel.setFont(juce::Font(16.0f, juce::Font::bold));
	addAndMakeVisible(nameLabel);

	volumeLabel.setText("Vol", juce::dontSendNotification);
	addAndMakeVisible(volumeLabel);

	speedLabel.setText("Speed", juce::dontSendNotification);
	addAndMakeVisible(speedLabel);

	timeLabel.setText("0:00 / 0:00", juce::dontSendNotification);
	timeLabel.setJustificationType(juce::Justification::centred);
	addAndMakeVisible(timeLabel);

	fileNameLabel.setText("No file loaded", juce::dontSendNotification);
	fileNameLabel.setJustificationType(juce::Justification::centred);
	fileNameLabel.setFont(juce::Font(13.0f, juce::Font::bold));
	addAndMakeVisible(fileNameLabel);

	metadataLabel.setText("", juce::dontSendNotification);
	metadataLabel.setJustificationType(juce::Justification::centredLeft);
	metadataLabel.setFont(juce::Font(11.0f));
	addAndMakeVisible(metadataLabel);

	addAndMakeVisible(waveformDisplay);

	applyThemeToComponents();
	startTimerHz(10);
}

PlayerGUI::~PlayerGUI()
{
	ThemeManager::getInstance().removeListener(this);
}

void PlayerGUI::themeChanged()
{
	applyThemeToComponents();
	repaint();
}

void PlayerGUI::applyThemeToComponents()
{
	auto& colors = ThemeManager::getInstance().getColors();

	styleButton(playButton, colors.playButton);
	styleButton(pauseButton, colors.pauseButton);
	styleButton(restartButton, juce::Colour(0xff9b59b6));
	styleButton(stopButton, colors.stopButton);
	styleButton(muteButton, colors.muteButton);
	styleButton(loopButton, colors.loopButton);
	styleButton(setAButton, juce::Colour(0xffe67e22));
	styleButton(setBButton, juce::Colour(0xffe67e22));
	styleButton(clearLoopButton, juce::Colour(0xffc0392b));
	styleButton(back10sButton, juce::Colour(0xff7f8c8d));
	styleButton(forward10sButton, juce::Colour(0xff7f8c8d));

	volumeSlider.setColour(juce::Slider::thumbColourId, colors.sliderThumb);
	volumeSlider.setColour(juce::Slider::trackColourId, colors.sliderTrack);
	volumeSlider.setColour(juce::Slider::textBoxTextColourId, colors.text);
	positionSlider.setColour(juce::Slider::thumbColourId, colors.text);
	positionSlider.setColour(juce::Slider::trackColourId, colors.sliderTrack);
	speedSlider.setColour(juce::Slider::thumbColourId, juce::Colour(0xff9b59b6));
	speedSlider.setColour(juce::Slider::trackColourId, juce::Colour(0xff9b59b6));
	speedSlider.setColour(juce::Slider::textBoxTextColourId, colors.text);

	nameLabel.setColour(juce::Label::textColourId, colors.accent);
	volumeLabel.setColour(juce::Label::textColourId, colors.text);
	speedLabel.setColour(juce::Label::textColourId, colors.text);
	timeLabel.setColour(juce::Label::textColourId, colors.text);
	fileNameLabel.setColour(juce::Label::textColourId, colors.textSecondary);
	metadataLabel.setColour(juce::Label::textColourId, colors.textSecondary);
}

void PlayerGUI::styleButton(juce::TextButton& button, juce::Colour colour)
{
	button.setColour(juce::TextButton::buttonColourId, colour);
	button.setColour(juce::TextButton::buttonOnColourId, colour.brighter(0.2f));
	button.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
}

void PlayerGUI::resized()
{
	auto area = getLocalBounds().reduced(5);

	nameLabel.setBounds(area.removeFromTop(20));
	area.removeFromTop(3);

	fileNameLabel.setBounds(area.removeFromTop(18));
	area.removeFromTop(2);

	metadataLabel.setBounds(area.removeFromTop(30));
	area.removeFromTop(3);

	waveformDisplay.setBounds(area.removeFromTop(60));
	area.removeFromTop(5);

	auto posArea = area.removeFromTop(20);
	back10sButton.setBounds(posArea.removeFromLeft(35));
	posArea.removeFromLeft(3);
	forward10sButton.setBounds(posArea.removeFromRight(35));
	posArea.removeFromRight(3);
	positionSlider.setBounds(posArea);
	area.removeFromTop(3);

	timeLabel.setBounds(area.removeFromTop(18));
	area.removeFromTop(5);

	auto ctrlArea = area.removeFromTop(30);
	int btnW = 55;
	playButton.setBounds(ctrlArea.removeFromLeft(btnW));
	ctrlArea.removeFromLeft(3);
	restartButton.setBounds(ctrlArea.removeFromLeft(btnW));
	ctrlArea.removeFromLeft(3);
	pauseButton.setBounds(ctrlArea.removeFromLeft(btnW));
	ctrlArea.removeFromLeft(3);
	stopButton.setBounds(ctrlArea.removeFromLeft(btnW));
	ctrlArea.removeFromLeft(3);
	muteButton.setBounds(ctrlArea.removeFromLeft(btnW));
	ctrlArea.removeFromLeft(3);
	loopButton.setBounds(ctrlArea.removeFromLeft(btnW));
	area.removeFromTop(5);

	auto loopArea = area.removeFromTop(25);
	setAButton.setBounds(loopArea.removeFromLeft(55));
	loopArea.removeFromLeft(3);
	setBButton.setBounds(loopArea.removeFromLeft(55));
	loopArea.removeFromLeft(3);
	clearLoopButton.setBounds(loopArea.removeFromLeft(75));
	area.removeFromTop(5);

	auto volArea = area.removeFromTop(25);
	volumeLabel.setBounds(volArea.removeFromLeft(30));
	volumeSlider.setBounds(volArea);
	area.removeFromTop(3);

	auto speedArea = area.removeFromTop(25);
	speedLabel.setBounds(speedArea.removeFromLeft(45));
	speedSlider.setBounds(speedArea);
}

bool PlayerGUI::loadFile(const juce::File& file)
{
	if (playerAudio.loadFile(file))
	{
		fileNameLabel.setText(playerAudio.getFileName(), juce::dontSendNotification);

		juce::String metadata;
		if (playerAudio.getTitle() != playerAudio.getFileName())
			metadata << "Title: " << playerAudio.getTitle() << "\n";
		if (playerAudio.getArtist() != "Unknown Artist")
			metadata << "Artist: " << playerAudio.getArtist();

		if (metadata.isEmpty())
			metadata = "No metadata";

		metadataLabel.setText(metadata, juce::dontSendNotification);
		return true;
	}
	return false;
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
	if (button == &playButton)
		playerAudio.start();
	else if (button == &restartButton) {
		playerAudio.setPosition(0.0); playerAudio.start();
	}
	else if (button == &pauseButton)
	{
		playerAudio.pause();
		pauseButton.setButtonText(playerAudio.isPaused() ? "Resume" : "Pause");
	}
	else if (button == &stopButton)
	{
		playerAudio.stop();
		pauseButton.setButtonText("Pause");
	}
	else if (button == &muteButton)
	{
		bool muted = !playerAudio.getMuted();
		if (muted)
			currentVolume = volumeSlider.getValue();
		playerAudio.setMute(muted);
		muteButton.setButtonText(muted ? "Unmute" : "Mute");
		volumeSlider.setValue(muted ? 0 : currentVolume);
	}
	else if (button == &loopButton)
	{
		bool looping = !playerAudio.getLooping();
		playerAudio.setLooping(looping);
		loopButton.setButtonText(looping ? "Looping" : "Loop");
	}
	else if (button == &setAButton)
		loopStart = playerAudio.getCurrentPosition();
	else if (button == &setBButton)
	{
		loopEnd = playerAudio.getCurrentPosition();
		if (loopEnd > loopStart)
			playerAudio.setLoopPoints(loopStart, loopEnd);
	}
	else if (button == &clearLoopButton)
		playerAudio.clearLoopPoints();
	else if (button == &back10sButton)
	{
		double newPos = playerAudio.getPosition() - 10.0;
		playerAudio.setPosition(newPos < 0.0 ? 0.0 : newPos);
	}
	else if (button == &forward10sButton)
	{
		double newPos = playerAudio.getPosition() + 10.0;
		playerAudio.setPosition(newPos > playerAudio.getLength() ? playerAudio.getLength() : newPos);
	}
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
	if (slider == &volumeSlider)
		playerAudio.setGain((float)slider->getValue());
	else if (slider == &positionSlider && playerAudio.getLength() > 0)
		playerAudio.setPosition(slider->getValue() * playerAudio.getLength());
	else if (slider == &speedSlider)
		playerAudio.setPlaybackSpeed(speedSlider.getValue());
}

juce::String PlayerGUI::formatTime(double seconds)
{
	int totalSec = (int)seconds;
	int mins = totalSec / 60;
	int secs = totalSec % 60;
	return juce::String(mins) + ":" + juce::String(secs).paddedLeft('0', 2);
}

void PlayerGUI::timerCallback()
{
	double total = playerAudio.getLength();
	double current = playerAudio.getPosition();

	if (total > 0)
	{
		positionSlider.setValue(current / total, juce::dontSendNotification);
		timeLabel.setText(formatTime(current) + " / " + formatTime(total), juce::dontSendNotification);
	}
}