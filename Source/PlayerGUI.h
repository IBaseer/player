#pragma once
#include <JuceHeader.h>

// Forward declaration فقط
class PlayerAudio;

class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::AudioSource
{
public:
    PlayerGUI();
    ~PlayerGUI() override;

    void resized() override;
    void paint(juce::Graphics& g) override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    // الدوال الإضافية التي أضفتها
    void setPlayerName(const juce::String& name) { playerName = name; }
    juce::String getPlayerName() const { return playerName; }

private:
    // استخدام unique_ptr لـ PlayerAudio
    std::unique_ptr<PlayerAudio> playerAudio;
    juce::String playerName = "Player";

    // كل عناصر واجهة المستخدم كما هي
    juce::TextButton loadButton{ "Load File" };
    juce::TextButton loadPlaylistButton{ "Load Playlist" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton playButton{ "Play" };
    juce::TextButton startButton{ "|< Start" };
    juce::TextButton endButton{ ">| End" };
    juce::TextButton loopButton{ "Loop" };
    juce::TextButton muteButton{ "Mute" };
    juce::TextButton prevButton{ "Prev" };
    juce::TextButton nextButton{ "Next" };
    juce::TextButton soloButton{ "Solo" };

    bool isLooping = false;
    bool muted = false;
    bool soloed = false;
    float CurrentVolume = 0;

    juce::Slider volumeSlider;

    // Metadata display
    juce::Label metadataLabel;
    juce::Label playerNameLabel;

    // Playlist
    juce::ListBox playlistBox;
    juce::StringArray playlistFiles;
    juce::Array<juce::File> playlistFileObjects;
    int currentPlaylistIndex = -1;

    std::unique_ptr<juce::FileChooser> fileChooser;

    // Event handlers 
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    // Playlist management
    void loadPlaylist();
    void addToPlaylist(const juce::Array<juce::File>& files);
    void playFromPlaylist(int index);
    void updateMetadataDisplay();

    // Custom list box model for playlist
    class PlaylistModel : public juce::ListBoxModel
    {
    public:
        PlaylistModel(PlayerGUI& owner) : owner(owner) {}

        int getNumRows() override { return owner.playlistFiles.size(); }

        void paintListBoxItem(int rowNumber, juce::Graphics& g,
            int width, int height, bool rowIsSelected) override;

        void listBoxItemClicked(int row, const juce::MouseEvent& event) override;

    private:
        PlayerGUI& owner;
    };

    std::unique_ptr<PlaylistModel> playlistModel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI);
};