#pragma once
#include <JuceHeader.h>

class PlayerAudio
{
public:
	PlayerAudio();
	~PlayerAudio();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
	void releaseResources();

	bool loadFile(const juce::File& file);
	void start();
	void stop();
	void pause();
	void setGain(float gain);
	float getGain() const { return currentGain; }

	double getPosition() const;
	double getLength() const;

	void setPosition(double newPosition);

	void setMute(bool shouldMute);
	bool getMuted() const { return muted; }

	void setLooping(bool shouldLoop);
	bool getLooping() const { return isLooping; }

	bool isPlaying() const;
	bool isPaused() const { return paused; }

	double getLengthInSeconds() const;
	double getCurrentPosition() const;

	void setLoopPoints(double start, double end);
	void clearLoopPoints();

	void setPlaybackSpeed(double speed);
	double getPlaybackSpeed() const { return playbackSpeed; }

	// Metadata retrieval
	juce::String getMetadata() const;
	juce::String getTitle() const { return metadata.title; }
	juce::String getArtist() const { return metadata.artist; }
	juce::String getAlbum() const { return metadata.album; }
	juce::String getFileName() const { return currentFileName; }

	// Waveform data
	void createWaveformThumbnail(const juce::File& file);
	juce::AudioThumbnail* getThumbnail() { return thumbnail.get(); }

	// Fade in/out
	void setFadeIn(bool shouldFade) { fadeInEnabled = shouldFade; }
	void setFadeOut(bool shouldFade) { fadeOutEnabled = shouldFade; }

	bool hasFileLoaded() const { return readerSource != nullptr; }

private:
	bool muted = false;
	bool isLooping = false;
	bool paused = false;

	double loopStart = 0.0;
	double loopEnd = 0.0;
	bool segmentLooping = false;
	double playbackSpeed = 1.0;
	float currentGain = 0.7f;

	// Fade settings
	bool fadeInEnabled = true;
	bool fadeOutEnabled = true;
	int fadeSamples = 0;
	int fadeCounter = 0;
	static const int FADE_LENGTH_SAMPLES = 4410; // ~100ms at 44.1kHz

	juce::String currentFileName;

	struct Metadata
	{
		juce::String title;
		juce::String artist;
		juce::String album;
		double duration = 0.0;
	} metadata;

	juce::AudioFormatManager formatManager;
	std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
	juce::AudioTransportSource transportSource;

	std::unique_ptr<juce::AudioThumbnailCache> thumbnailCache;
	std::unique_ptr<juce::AudioThumbnail> thumbnail;

	void applyFade(const juce::AudioSourceChannelInfo& bufferToFill);
};

// ==================== THEME MANAGER ====================

class ThemeManager
{
public:
	enum class Theme { Dark, Light };

	static ThemeManager& getInstance()
	{
		static ThemeManager instance;
		return instance;
	}

	struct ColorScheme
	{
		juce::Colour background;
		juce::Colour secondaryBackground;
		juce::Colour border;
		juce::Colour text;
		juce::Colour textSecondary;
		juce::Colour accent;
		juce::Colour waveform;
		juce::Colour playButton;
		juce::Colour pauseButton;
		juce::Colour stopButton;
		juce::Colour muteButton;
		juce::Colour loopButton;
		juce::Colour sliderThumb;
		juce::Colour sliderTrack;
		juce::Colour tableBackground;
		juce::Colour tableRow;
		juce::Colour tableRowAlt;
		juce::Colour tableSelected;
	};

	void setTheme(Theme newTheme)
	{
		currentTheme = newTheme;
		for (auto* listener : listeners)
			listener->themeChanged();
	}

	Theme getCurrentTheme() const { return currentTheme; }

	const ColorScheme& getColors() const
	{
		return currentTheme == Theme::Dark ? darkColors : lightColors;
	}

	class Listener
	{
	public:
		virtual ~Listener() = default;
		virtual void themeChanged() = 0;
	};

	void addListener(Listener* listener)
	{
		listeners.insert(listener);
	}

	void removeListener(Listener* listener)
	{
		listeners.erase(listener);
	}

private:
	ThemeManager()
	{
		// Dark Theme (Studio Look)
		darkColors.background = juce::Colour(0xff0a0a0a);
		darkColors.secondaryBackground = juce::Colour(0xff1e1e1e);
		darkColors.border = juce::Colour(0xff2a2a2a);
		darkColors.text = juce::Colours::white;
		darkColors.textSecondary = juce::Colour(0xffcccccc);
		darkColors.accent = juce::Colour(0xff4a9eff);
		darkColors.waveform = juce::Colour(0xff4a9eff);
		darkColors.playButton = juce::Colour(0xff2ecc71);
		darkColors.pauseButton = juce::Colour(0xfff39c12);
		darkColors.stopButton = juce::Colour(0xffe74c3c);
		darkColors.muteButton = juce::Colour(0xff95a5a6);
		darkColors.loopButton = juce::Colour(0xff16a085);
		darkColors.sliderThumb = juce::Colour(0xff4a9eff);
		darkColors.sliderTrack = juce::Colour(0xff4a9eff);
		darkColors.tableBackground = juce::Colour(0xff1a1a1a);
		darkColors.tableRow = juce::Colour(0xff1e1e1e);
		darkColors.tableRowAlt = juce::Colour(0xff252525);
		darkColors.tableSelected = juce::Colour(0xff3a3a3a);

		// Light Theme (Modern Clean)
		lightColors.background = juce::Colour(0xfff5f5f5);
		lightColors.secondaryBackground = juce::Colour(0xffffffff);
		lightColors.border = juce::Colour(0xffdcdcdc);
		lightColors.text = juce::Colour(0xff1a1a1a);
		lightColors.textSecondary = juce::Colour(0xff666666);
		lightColors.accent = juce::Colour(0xff2196F3);
		lightColors.waveform = juce::Colour(0xff2196F3);
		lightColors.playButton = juce::Colour(0xff4CAF50);
		lightColors.pauseButton = juce::Colour(0xffFF9800);
		lightColors.stopButton = juce::Colour(0xffF44336);
		lightColors.muteButton = juce::Colour(0xff9E9E9E);
		lightColors.loopButton = juce::Colour(0xff009688);
		lightColors.sliderThumb = juce::Colour(0xff2196F3);
		lightColors.sliderTrack = juce::Colour(0xff2196F3);
		lightColors.tableBackground = juce::Colour(0xffffffff);
		lightColors.tableRow = juce::Colour(0xfffafafa);
		lightColors.tableRowAlt = juce::Colour(0xfff0f0f0);
		lightColors.tableSelected = juce::Colour(0xffe3f2fd);
	}

	Theme currentTheme = Theme::Dark;
	ColorScheme darkColors;
	ColorScheme lightColors;
	std::set<Listener*> listeners;
};