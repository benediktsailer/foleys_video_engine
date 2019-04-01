
#pragma once


namespace foleys
{

class AVFormatManager
{
public:
    enum class Formats
    {
        PROBE = 0,
        FFmpeg
    };

    static std::unique_ptr<AVClip> createClipFromFile (juce::File file);

    static std::unique_ptr<AVReader> createReaderFor (juce::File file, StreamTypes type = StreamTypes::all());


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AVFormatManager)
};

}
