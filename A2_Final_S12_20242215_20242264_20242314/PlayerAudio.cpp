#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
	formatManager.registerBasicFormats();

	thumbnailCache = std::make_unique<juce::AudioThumbnailCache>(5);
	thumbnail = std::make_unique<juce::AudioThumbnail>(512, formatManager, *thumbnailCache);
}

PlayerAudio::~PlayerAudio()
{
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::releaseResources()
{
	transportSource.releaseResources();
}

bool PlayerAudio::loadFile(const juce::File& file)
{
	if (file.existsAsFile())
	{
		if (auto* reader = formatManager.createReaderFor(file))
		{
			transportSource.stop();
			transportSource.setSource(nullptr);
			readerSource.reset();

			currentFileName = file.getFileNameWithoutExtension();

			metadata.title = reader->metadataValues.getValue("title", currentFileName);
			metadata.artist = reader->metadataValues.getValue("artist", "Unknown Artist");
			metadata.album = reader->metadataValues.getValue("album", "Unknown Album");
			metadata.duration = reader->lengthInSamples / reader->sampleRate;

			readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

			transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);

			thumbnail->setSource(new juce::FileInputSource(file));

			paused = false;
			fadeCounter = 0;
			return true;
		}
	}
	return false;
}

void PlayerAudio::start()
{
	transportSource.start();
	paused = false;
	fadeCounter = 0;
}

void PlayerAudio::stop()
{
	transportSource.stop();
	paused = false;
	fadeCounter = 0;
}

void PlayerAudio::pause()
{
	if (transportSource.isPlaying())
	{
		transportSource.stop();
		paused = true;
	}
	else if (paused)
	{
		transportSource.start();
		paused = false;
		fadeCounter = 0;
	}
}

void PlayerAudio::setGain(float gain)
{
	currentGain = gain;
	transportSource.setGain(gain);
}

double PlayerAudio::getPosition() const
{
	return transportSource.getCurrentPosition();
}

double PlayerAudio::getLength() const
{
	return transportSource.getLengthInSeconds();
}

void PlayerAudio::setMute(bool shouldMute)
{
	muted = shouldMute;
}

void PlayerAudio::setLooping(bool shouldLoop)
{
	isLooping = shouldLoop;

	if (readerSource != nullptr)
		readerSource->setLooping(isLooping);
}

bool PlayerAudio::isPlaying() const
{
	return transportSource.isPlaying();
}

double PlayerAudio::getLengthInSeconds() const
{
	return transportSource.getLengthInSeconds();
}

double PlayerAudio::getCurrentPosition() const
{
	return transportSource.getCurrentPosition();
}

void PlayerAudio::setPosition(double newPosition)
{
	transportSource.setPosition(newPosition);
	fadeCounter = 0;
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	if (readerSource == nullptr)
	{
		bufferToFill.clearActiveBufferRegion();
		return;
	}

	transportSource.getNextAudioBlock(bufferToFill);

	// A-B loop
	if (segmentLooping && transportSource.getCurrentPosition() >= loopEnd)
	{
		transportSource.setPosition(loopStart);
		fadeCounter = 0;
	}

	// Apply fade in/out
	applyFade(bufferToFill);
}

void PlayerAudio::applyFade(const juce::AudioSourceChannelInfo& bufferToFill)
{
	if (fadeInEnabled && fadeCounter < FADE_LENGTH_SAMPLES && transportSource.isPlaying())
	{
		int samplesToFade = juce::jmin(FADE_LENGTH_SAMPLES - fadeCounter, bufferToFill.numSamples);

		for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
		{
			float* channelData = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);

			for (int i = 0; i < samplesToFade; ++i)
			{
				float fadeFactor = (float)(fadeCounter + i) / (float)FADE_LENGTH_SAMPLES;
				channelData[i] *= fadeFactor;
			}
		}

		fadeCounter += samplesToFade;
	}
}

void PlayerAudio::setLoopPoints(double start, double end)
{
	loopStart = start;
	loopEnd = end;
	segmentLooping = true;
}

void PlayerAudio::clearLoopPoints()
{
	segmentLooping = false;
}

void PlayerAudio::setPlaybackSpeed(double speed)
{
	playbackSpeed = speed;
	transportSource.setSource(readerSource.get(), 0, nullptr,
		readerSource ? readerSource->getAudioFormatReader()->sampleRate * speed : 44100.0);
}

juce::String PlayerAudio::getMetadata() const
{
	juce::String info;
	info << "Title: " << metadata.title << "\n";
	info << "Artist: " << metadata.artist << "\n";
	info << "Album: " << metadata.album << "\n";
	info << "Duration: " << juce::String(metadata.duration, 2) << "s";
	return info;
}

void PlayerAudio::createWaveformThumbnail(const juce::File& file)
{
	thumbnail->setSource(new juce::FileInputSource(file));
}