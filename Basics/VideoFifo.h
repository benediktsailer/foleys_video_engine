/*
 ==============================================================================

 Copyright (c) 2019, Foleys Finest Audio - Daniel Walz
 All rights reserved.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 OF THE POSSIBILITY OF SUCH DAMAGE.

 ==============================================================================
 */

#pragma once

namespace foleys
{

class VideoFifo
{
public:
    VideoFifo() = default;
    ~VideoFifo() = default;

    void pushVideoFrame (juce::Image& image, juce::int64 timestamp);
    std::pair<int64_t, juce::Image> popVideoFrame();

    std::pair<int64_t, juce::Image> getVideoFrame (double timestamp) const;
    bool isFrameAvailable (double timestamp) const;

    int getNumAvailableFrames() const;
    juce::int64 getLowestTimeCode() const;
    juce::int64 getHighestTimeCode() const;

    juce::Image getOldestFrameForRecycling();

    Timecode getFrameTimecodeForTime (double time) const;

    size_t size() const;

    void clear (juce::int64 count);

    void clearFramesOlderThan (Timecode timecode);

    VideoStreamSettings& getVideoSettings();

private:
    juce::CriticalSection lock;

    VideoStreamSettings settings;

    std::map<juce::int64, juce::Image> videoFrames;
    juce::int64 lastViewedFrame = -1;
    bool reverse = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VideoFifo)
};


} // foleys
