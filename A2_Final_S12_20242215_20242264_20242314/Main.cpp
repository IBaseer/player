#include <JuceHeader.h>
#include "MainComponent.h"

class EnhancedAudioPlayer : public juce::JUCEApplication
{
public:
	const juce::String getApplicationName() override { return "Audio Player Pro"; }
	const juce::String getApplicationVersion() override { return "1.0"; }

	void initialise(const juce::String&) override
	{
		mainWindow = std::make_unique<MainWindow>(getApplicationName());
	}

	void shutdown() override
	{
		mainWindow = nullptr;
	}

private:
	class MainWindow : public juce::DocumentWindow
	{
	public:
		MainWindow(juce::String name)
			: DocumentWindow(name,
				juce::Colour(0xff0a0a0a),
				DocumentWindow::allButtons)
		{
			setUsingNativeTitleBar(true);
			setContentOwned(new MainComponent(), true);
			centreWithSize(1100, 700);
			setResizeLimits(900, 650, 2000, 2000);
			setVisible(true);
			setResizable(true, true);
		}

		void closeButtonPressed() override
		{
			juce::JUCEApplication::getInstance()->systemRequestedQuit();
		}
	};

	std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION(EnhancedAudioPlayer)