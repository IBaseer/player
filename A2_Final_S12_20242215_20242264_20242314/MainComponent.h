#pragma once
#include <JuceHeader.h>
#include "PlayerGUI.h"

class PlaylistComponent : public juce::Component,
	public juce::TableListBoxModel,
	public juce::Button::Listener,
	public ThemeManager::Listener
{
public:
	PlaylistComponent();
	~PlaylistComponent() override;

	void paint(juce::Graphics& g) override;
	void resized() override;
	void themeChanged() override;

	int getNumRows() override;
	void paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
	void paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
	void cellClicked(int rowNumber, int columnId, const juce::MouseEvent& e) override;

	void addFiles();
	void clearPlaylist();

	void setPlayer1Callback(std::function<void(const juce::File&)> callback) { loadToPlayer1 = callback; }
	void setPlayer2Callback(std::function<void(const juce::File&)> callback) { loadToPlayer2 = callback; }


	void playNext();
	void playPrevious();

	int getCurrentTrackIndex() const { return currentTrackIndex; }
	int getPlaylistSize() const { return playlist.size(); }

private:
	struct TrackInfo
	{
		juce::File file;
		juce::String title;
		juce::String artist;
		double duration;
	};

	std::vector<TrackInfo> playlist;
	std::vector<int> shuffledIndices;
	int currentTrackIndex = -1;

	bool shuffleEnabled = false;
	bool repeatEnabled = false;
	bool autoNextEnabled = false;

	juce::TableListBox table;

	juce::TextButton addButton{ "Add Files" };
	juce::TextButton clearButton{ "Clear All" };
	juce::TextButton nextButton{ "Next >>" };
	juce::TextButton prevButton{ "<< Prev" };

	juce::Label titleLabel;
	juce::Label statsLabel;

	std::function<void(const juce::File&)> loadToPlayer1;
	std::function<void(const juce::File&)> loadToPlayer2;

	std::unique_ptr<juce::FileChooser> fileChooser;

	void buttonClicked(juce::Button* button) override;
	void updateStatsLabel();
	TrackInfo extractTrackInfo(const juce::File& file);
	void applyThemeToComponents();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent);
};

class MainComponent : public juce::AudioAppComponent, public ThemeManager::Listener
{
public:
	MainComponent();
	~MainComponent() override;

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;

	void resized() override;
	void paint(juce::Graphics& g) override;
	void themeChanged() override;

private:
	PlayerGUI player1;
	PlayerGUI player2;
	PlaylistComponent playlist;

	juce::Label titleLabel;
	juce::Slider mixerSlider;
	juce::Label mixerLabel;

	juce::TextButton themeToggleButton{ "Light Mode" };

	void applyThemeToComponents();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};