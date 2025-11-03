#include "PlayerGUI.h"
#include "PlayerAudio.h" // التضمين هنا فقط

// تنفيذ دوال PlaylistModel هنا
void PlayerGUI::PlaylistModel::paintListBoxItem(int rowNumber, juce::Graphics& g,
    int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll(juce::Colours::lightblue);

    g.setColour(juce::Colours::white);
    g.setFont(14.0f);

    if (rowNumber < owner.playlistFiles.size())
        g.drawText(owner.playlistFiles[rowNumber], 10, 0, width - 10, height,
            juce::Justification::centredLeft);
}

void PlayerGUI::PlaylistModel::listBoxItemClicked(int row, const juce::MouseEvent&)
{
    owner.playFromPlaylist(row);
}

PlayerGUI::PlayerGUI()
{
    // إنشاء PlayerAudio
    playerAudio = std::make_unique<PlayerAudio>();

    // Add buttons
    for (auto* btn : { &loadButton, &loadPlaylistButton, &restartButton, &playButton,
                      &stopButton, &startButton, &endButton, &loopButton, &muteButton,
                      &prevButton, &nextButton, &soloButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    // Volume slider
    volumeSlider.setRange(0.0, 2, 0.10);
    volumeSlider.setValue(0.7);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

    // Player name label
    playerNameLabel.setColour(juce::Label::textColourId, juce::Colours::yellow);
    playerNameLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    playerNameLabel.setText(playerName, juce::dontSendNotification);
    addAndMakeVisible(playerNameLabel);

    // Metadata label
    metadataLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    metadataLabel.setFont(juce::Font(14.0f));
    metadataLabel.setText("No file loaded", juce::dontSendNotification);
    addAndMakeVisible(metadataLabel);

    // Playlist
    playlistModel = std::make_unique<PlaylistModel>(*this);
    playlistBox.setModel(playlistModel.get());
    playlistBox.setColour(juce::ListBox::backgroundColourId, juce::Colours::darkgrey);
    playlistBox.setColour(juce::ListBox::textColourId, juce::Colours::white);
    addAndMakeVisible(playlistBox);
}

PlayerGUI::~PlayerGUI()
{
}

void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio->prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio->getNextAudioBlock(bufferToFill);
}

void PlayerGUI::releaseResources()
{
    playerAudio->releaseResources();
}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);

    // رسم حدود حول الـplayer
    g.setColour(juce::Colours::lightgrey);
    g.drawRect(getLocalBounds(), 2);
}

void PlayerGUI::resized()
{
    auto area = getLocalBounds();

    // Player name at the top
    auto nameArea = area.removeFromTop(25);
    playerNameLabel.setBounds(nameArea.reduced(5));

    // Top area for metadata
    auto metadataArea = area.removeFromTop(80);
    metadataLabel.setBounds(metadataArea.reduced(10));

    // Control buttons area
    auto buttonArea = area.removeFromTop(60);
    int buttonWidth = 65; // تصغير الأزرار قليلاً لتناسب المساحة

    loadButton.setBounds(buttonArea.removeFromLeft(80));
    loadPlaylistButton.setBounds(buttonArea.removeFromLeft(90));
    buttonArea.removeFromLeft(5);
    prevButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
    nextButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
    buttonArea.removeFromLeft(5);
    restartButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
    playButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
    stopButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
    startButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
    endButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
    muteButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
    soloButton.setBounds(buttonArea.removeFromLeft(buttonWidth));
    loopButton.setBounds(buttonArea.removeFromLeft(buttonWidth));

    // Volume slider area
    auto volumeArea = area.removeFromTop(40);
    volumeSlider.setBounds(volumeArea.reduced(10, 5));

    // Playlist area (remaining space)
    playlistBox.setBounds(area.reduced(5));
}
