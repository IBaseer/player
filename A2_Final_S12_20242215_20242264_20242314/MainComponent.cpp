#include "MainComponent.h"

// ==================== PlaylistComponent ====================

PlaylistComponent::PlaylistComponent()
{
	ThemeManager::getInstance().addListener(this);

	addAndMakeVisible(table);
	table.setModel(this);
	table.getHeader().addColumn("Track", 1, 250);
	table.getHeader().addColumn("Artist", 2, 150);
	table.getHeader().addColumn("Duration", 3, 80);
	table.getHeader().addColumn("Player 1", 4, 70);
	table.getHeader().addColumn("Player 2", 5, 70);

	for (auto* btn : { &addButton, &clearButton, &nextButton, &prevButton })
	{
		btn->addListener(this);
		addAndMakeVisible(btn);
	}

	titleLabel.setText("PLAYLIST MANAGER", juce::dontSendNotification);
	titleLabel.setJustificationType(juce::Justification::centred);
	titleLabel.setFont(juce::Font(18.0f, juce::Font::bold));
	addAndMakeVisible(titleLabel);

	statsLabel.setText("0 tracks", juce::dontSendNotification);
	addAndMakeVisible(statsLabel);

	applyThemeToComponents();
}

PlaylistComponent::~PlaylistComponent()
{
	ThemeManager::getInstance().removeListener(this);
}

void PlaylistComponent::themeChanged()
{
	applyThemeToComponents();
	repaint();
	table.repaint();
}

void PlaylistComponent::applyThemeToComponents()
{
	auto& colors = ThemeManager::getInstance().getColors();

	table.setColour(juce::ListBox::backgroundColourId, colors.tableBackground);
	addButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff4a9eff));
	addButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
	clearButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xffe74c3c));
	clearButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
	nextButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff7f8c8d));
	nextButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
	prevButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff7f8c8d));
	prevButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);

	titleLabel.setColour(juce::Label::textColourId, colors.accent);
	statsLabel.setColour(juce::Label::textColourId, colors.textSecondary);
}

void PlaylistComponent::paint(juce::Graphics& g)
{
	auto& colors = ThemeManager::getInstance().getColors();
	g.fillAll(colors.background);
}

void PlaylistComponent::resized()
{
	auto area = getLocalBounds().reduced(5);

	titleLabel.setBounds(area.removeFromTop(25));
	area.removeFromTop(5);

	auto btnArea = area.removeFromTop(30);
	addButton.setBounds(btnArea.removeFromLeft(80));
	btnArea.removeFromLeft(5);
	clearButton.setBounds(btnArea.removeFromLeft(80));
	btnArea.removeFromLeft(10);
	prevButton.setBounds(btnArea.removeFromLeft(70));
	btnArea.removeFromLeft(5);
	nextButton.setBounds(btnArea.removeFromLeft(70));

	area.removeFromTop(5);
	statsLabel.setBounds(area.removeFromTop(20));
	area.removeFromTop(3);

	table.setBounds(area);
}

int PlaylistComponent::getNumRows()
{
	return (int)playlist.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
	auto& colors = ThemeManager::getInstance().getColors();

	if (rowIsSelected)
		g.fillAll(colors.tableSelected);
	else if (rowNumber == currentTrackIndex)
		g.fillAll(juce::Colour(0xff2a4a2a));
	else if (rowNumber % 2 == 0)
		g.fillAll(colors.tableRow);
	else
		g.fillAll(colors.tableRowAlt);
}

void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
	if (rowNumber >= playlist.size())
		return;

	auto& colors = ThemeManager::getInstance().getColors();
	g.setColour(rowIsSelected ? colors.text : colors.textSecondary);
	g.setFont(12.0f);

	const auto& track = playlist[rowNumber];
	juce::String text;

	switch (columnId)
	{
	case 1: text = track.title; break;
	case 2: text = track.artist; break;
	case 3:
	{
		int mins = (int)track.duration / 60;
		int secs = (int)track.duration % 60;
		text = juce::String(mins) + ":" + juce::String(secs).paddedLeft('0', 2);
		break;
	}
	case 4: text = "Load P1"; break;
	case 5: text = "Load P2"; break;
	}

	g.drawText(text, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
}

void PlaylistComponent::cellClicked(int rowNumber, int columnId, const juce::MouseEvent& e)
{
	if (rowNumber >= playlist.size())
		return;

	const auto& track = playlist[rowNumber];

	if (columnId == 4 && loadToPlayer1)
	{
		loadToPlayer1(track.file);
		currentTrackIndex = rowNumber;
	}
	else if (columnId == 5 && loadToPlayer2)
	{
		loadToPlayer2(track.file);
		currentTrackIndex = rowNumber;
	}

	table.repaint();
}

void PlaylistComponent::addFiles()
{
	fileChooser = std::make_unique<juce::FileChooser>(
		"Select audio files...",
		juce::File{},
		"*.wav;*.mp3;*.flac;*.ogg;*.aiff");

	fileChooser->launchAsync(
		juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles | juce::FileBrowserComponent::canSelectMultipleItems,
		[this](const juce::FileChooser& fc)
		{
			auto files = fc.getResults();
			for (const auto& file : files)
			{
				if (file.existsAsFile())
				{
					playlist.push_back(extractTrackInfo(file));
				}
			}
			updateStatsLabel();
			table.updateContent();
		});
}

void PlaylistComponent::clearPlaylist()
{
	playlist.clear();
	currentTrackIndex = -1;
	updateStatsLabel();
	table.updateContent();
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
	if (button == &addButton)
		addFiles();
	else if (button == &clearButton)
		clearPlaylist();
	else if (button == &nextButton)
		playNext();
	else if (button == &prevButton)
		playPrevious();
}

void PlaylistComponent::updateStatsLabel()
{
	double totalDuration = 0;
	for (const auto& track : playlist)
		totalDuration += track.duration;

	int mins = (int)totalDuration / 60;
	juce::String stats = juce::String(playlist.size()) + " tracks (" + juce::String(mins) + " minutes)";
	statsLabel.setText(stats, juce::dontSendNotification);
}

PlaylistComponent::TrackInfo PlaylistComponent::extractTrackInfo(const juce::File& file)
{
	TrackInfo info;
	info.file = file;
	info.title = file.getFileNameWithoutExtension();
	info.artist = "Unknown";
	info.duration = 0.0;

	juce::AudioFormatManager formatManager;
	formatManager.registerBasicFormats();

	if (auto* reader = formatManager.createReaderFor(file))
	{
		info.title = reader->metadataValues.getValue("title", info.title);
		info.artist = reader->metadataValues.getValue("artist", "Unknown");
		info.duration = reader->lengthInSamples / reader->sampleRate;
		delete reader;
	}

	return info;
}

void PlaylistComponent::playNext()
{
	if (playlist.empty())
		return;

	if (shuffleEnabled && !shuffledIndices.empty())
	{
		auto it = std::find(shuffledIndices.begin(), shuffledIndices.end(), currentTrackIndex);
		if (it != shuffledIndices.end() && ++it != shuffledIndices.end())
			currentTrackIndex = *it;
		else if (repeatEnabled)
			currentTrackIndex = shuffledIndices[0];
		else
			return;
	}
	else
	{
		currentTrackIndex++;
		if (currentTrackIndex >= playlist.size())
		{
			if (repeatEnabled)
				currentTrackIndex = 0;
			else
				return;
		}
	}

	table.repaint();
}

void PlaylistComponent::playPrevious()
{
	if (playlist.empty())
		return;

	if (shuffleEnabled && !shuffledIndices.empty())
	{
		auto it = std::find(shuffledIndices.begin(), shuffledIndices.end(), currentTrackIndex);
		if (it != shuffledIndices.begin())
			currentTrackIndex = *(--it);
		else if (repeatEnabled)
			currentTrackIndex = shuffledIndices.back();
	}
	else
	{
		currentTrackIndex--;
		if (currentTrackIndex < 0)
		{
			if (repeatEnabled)
				currentTrackIndex = playlist.size() - 1;
			else
				currentTrackIndex = 0;
		}
	}

	table.repaint();
}

// ==================== MainComponent ====================

MainComponent::MainComponent()
	: player1("PLAYER 1"), player2("PLAYER 2")
{
	ThemeManager::getInstance().addListener(this);

	addAndMakeVisible(player1);
	addAndMakeVisible(player2);
	addAndMakeVisible(playlist);

	titleLabel.setText("DUAL AUDIO PLAYER - PROFESSIONAL EDITION", juce::dontSendNotification);
	titleLabel.setJustificationType(juce::Justification::centred);
	titleLabel.setFont(juce::Font(20.0f, juce::Font::bold));
	addAndMakeVisible(titleLabel);

	mixerLabel.setText("MIXER (L/R)", juce::dontSendNotification);
	mixerLabel.setJustificationType(juce::Justification::centred);
	mixerLabel.setFont(juce::Font(12.0f, juce::Font::bold));
	addAndMakeVisible(mixerLabel);

	mixerSlider.setRange(0.0, 1.0, 0.01);
	mixerSlider.setValue(0.5);
	mixerSlider.setSliderStyle(juce::Slider::LinearVertical);
	mixerSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
	addAndMakeVisible(mixerSlider);

	themeToggleButton.onClick = [this]()
		{
			auto& themeManager = ThemeManager::getInstance();
			if (themeManager.getCurrentTheme() == ThemeManager::Theme::Dark)
			{
				themeManager.setTheme(ThemeManager::Theme::Light);
				themeToggleButton.setButtonText("Dark Mode");
			}
			else
			{
				themeManager.setTheme(ThemeManager::Theme::Dark);
				themeToggleButton.setButtonText("Light Mode");
			}
		};
	addAndMakeVisible(themeToggleButton);

	applyThemeToComponents();

	playlist.setPlayer1Callback([this](const juce::File& file) {
		player1.loadFile(file);
		});

	playlist.setPlayer2Callback([this](const juce::File& file) {
		player2.loadFile(file);
		});

	setSize(1400, 900);
	setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
	ThemeManager::getInstance().removeListener(this);
	shutdownAudio();
}

void MainComponent::themeChanged()
{
	applyThemeToComponents();
	repaint();
}

void MainComponent::applyThemeToComponents()
{
	auto& colors = ThemeManager::getInstance().getColors();

	titleLabel.setColour(juce::Label::textColourId, colors.text);
	mixerLabel.setColour(juce::Label::textColourId, colors.accent);

	mixerSlider.setColour(juce::Slider::thumbColourId, colors.sliderThumb);
	mixerSlider.setColour(juce::Slider::trackColourId, colors.sliderTrack);
	mixerSlider.setColour(juce::Slider::backgroundColourId, colors.border);
	mixerSlider.setColour(juce::Slider::textBoxTextColourId, colors.text);
	mixerSlider.setColour(juce::Slider::textBoxBackgroundColourId, colors.secondaryBackground);
	mixerSlider.setColour(juce::Slider::textBoxOutlineColourId, colors.border);

	themeToggleButton.setColour(juce::TextButton::buttonColourId, colors.accent);
	themeToggleButton.setColour(juce::TextButton::textColourOffId, colors.text);


}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
	player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	bufferToFill.clearActiveBufferRegion();

	float mixRatio = mixerSlider.getValue();
	float player1Gain = 1.0f - mixRatio;
	float player2Gain = mixRatio;

	juce::AudioBuffer<float> tempBuffer1(bufferToFill.buffer->getNumChannels(), bufferToFill.numSamples);
	juce::AudioBuffer<float> tempBuffer2(bufferToFill.buffer->getNumChannels(), bufferToFill.numSamples);

	tempBuffer1.clear();
	tempBuffer2.clear();

	juce::AudioSourceChannelInfo tempInfo1(&tempBuffer1, 0, bufferToFill.numSamples);
	juce::AudioSourceChannelInfo tempInfo2(&tempBuffer2, 0, bufferToFill.numSamples);

	player1.getNextAudioBlock(tempInfo1);
	player2.getNextAudioBlock(tempInfo2);

	for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
	{
		float* outputData = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
		const float* input1 = tempBuffer1.getReadPointer(channel);
		const float* input2 = tempBuffer2.getReadPointer(channel);

		for (int i = 0; i < bufferToFill.numSamples; ++i)
		{
			outputData[i] = (input1[i] * player1Gain) + (input2[i] * player2Gain);
		}
	}
}

void MainComponent::releaseResources()
{
	player1.releaseResources();
	player2.releaseResources();
}

void MainComponent::paint(juce::Graphics& g)
{
	auto& colors = ThemeManager::getInstance().getColors();
	g.fillAll(colors.background);
}

void MainComponent::resized()
{
	auto area = getLocalBounds();

	// Title bar with theme toggle
	auto topBar = area.removeFromTop(35);
	themeToggleButton.setBounds(topBar.removeFromRight(120).reduced(2, 2));
	titleLabel.setBounds(topBar);

	area.removeFromTop(5);

	auto topSection = area.removeFromTop(350);

	auto mixerArea = topSection.removeFromRight(80);
	mixerLabel.setBounds(mixerArea.removeFromTop(25));
	mixerSlider.setBounds(mixerArea.reduced(10, 5));

	topSection.removeFromRight(5);

	auto player1Area = topSection.removeFromLeft(topSection.getWidth() / 2);
	player1Area.removeFromRight(5);
	player1.setBounds(player1Area);

	topSection.removeFromLeft(5);
	player2.setBounds(topSection);

	area.removeFromTop(10);
	playlist.setBounds(area);
}