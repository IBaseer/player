#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
}

PlayerAudio::~PlayerAudio()
{
    transportSource.setSource(nullptr);
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    if (muted)
    {
        // إذا كان مكتوم، إرجاع صمت
        bufferToFill.clearActiveBufferRegion();
    }
    else if (readerSource != nullptr)
    {
        // تشغيل الصوت العادي
        transportSource.getNextAudioBlock(bufferToFill);
    }
    else
    {
        // لا يوجد ملف محمل، إرجاع صمت
        bufferToFill.clearActiveBufferRegion();
    }
}

void PlayerAudio::releaseResources()
{
    transportSource.releaseResources();
}

bool PlayerAudio::loadFile(const juce::File& file)
{
    return loadFileFromPlaylist(file);
}

bool PlayerAudio::loadFileFromPlaylist(const juce::File& file)
{
    if (file.existsAsFile())
    {
        if (auto* reader = formatManager.createReaderFor(file))
        {
            // إيقاف وإعادة تعيين المصدر القديم
            transportSource.stop();
            transportSource.setSource(nullptr);
            readerSource.reset();

            // إنشاء مصدر القارئ الجديد
            readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

            // توصيل المصدر الجديد
            transportSource.setSource(readerSource.get(),
                0,
                nullptr,
                reader->sampleRate);
            
            // تخزين معلومات الملف واستخراج metadata
            currentFile = file;
            duration = transportSource.getLengthInSeconds();
            
            // استخراج metadata
            if (auto* metadataReader = dynamic_cast<juce::AudioFormatReader*>(reader))
            {
                // محاولة الحصول على metadata إذا كان متاحاً
                title = metadataReader->metadataValues.getValue("Title", "");
                artist = metadataReader->metadataValues.getValue("Artist", "");
                
                // إذا لم يوجد metadata، استخدام اسم الملف
                if (title.isEmpty())
                    title = currentFile.getFileNameWithoutExtension();
                if (artist.isEmpty())
                    artist = "Unknown Artist";
            }
            else
            {
                // الرجوع إلى اسم الملف
                title = currentFile.getFileNameWithoutExtension();
                artist = "Unknown Artist";
            }
            
            // تعيين حالة looping الحالية
            if (readerSource != nullptr)
                readerSource->setLooping(isLooping);
            
            return true;
        }
    }
    return false;
}

void PlayerAudio::start()
{
    if (readerSource != nullptr)
    {
        transportSource.start();
    }
}

void PlayerAudio::stop()
{
    transportSource.stop();
}

void PlayerAudio::setGain(float gain)
{
    if (!muted) // فقط تحديث gain إذا لم يكن مكتوماً
    {
        transportSource.setGain(gain);
    }
    // تخزين القيمة حتى لو كان مكتوماً لاستخدامها عند إلغاء الكتم
    previousGain = gain;
}

void PlayerAudio::setPosition(double pos)
{
    transportSource.setPosition(pos);
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
    if (muted == shouldMute) return;
    
    muted = shouldMute;
    if (muted)
    {
        previousGain = transportSource.getGain();
        transportSource.setGain(0.0f);
    }
    else
    {
        transportSource.setGain(previousGain);
    }
}

void PlayerAudio::setLooping(bool shouldLoop)
{
    isLooping = shouldLoop;
    if (readerSource != nullptr)
        readerSource->setLooping(isLooping);
}

// Metadata functions
juce::String PlayerAudio::getCurrentFileName() const
{
    return currentFile.getFileName();
}

juce::String PlayerAudio::getMetadata() const
{
    if (!hasFileLoaded())
        return "No file loaded";
    
    juce::String metadata;
    metadata += "Title: " + title + "\n";
    metadata += "Artist: " + artist + "\n";
    metadata += "Duration: " + juce::String::formatted("%.2f seconds", duration) + "\n";
    metadata += "File: " + currentFile.getFileName();
    
    return metadata;
}

juce::String PlayerAudio::getTitle() const 
{ 
    return title; 
}

juce::String PlayerAudio::getArtist() const 
{ 
    return artist; 
}

double PlayerAudio::getDuration() const 
{ 
    return duration; 
}