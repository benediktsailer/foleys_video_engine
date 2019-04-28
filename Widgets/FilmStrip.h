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

/**
 @class FilmStrip

 The FilmStrip allows to display a line of thumbnails of a video clip.
 */
class FilmStrip : public juce::Component
{
public:
    FilmStrip();
    virtual ~FilmStrip();

    /** Set the clip to be shown as thimbnails */
    void setClip (std::shared_ptr<AVClip> clip);

    /** @Internal */
    void paint (juce::Graphics&) override;
    /** @Internal */
    void resized() override;

    /** Set the start time and the end time of the clip in seconds. This
        is used to allow only a subset of thumbnails to be shown. */
    void setStartAndLength (double start, double length);

    /** @Internal */
    class ThumbnailJob : public juce::ThreadPoolJob
    {
    public:
        ThumbnailJob (FilmStrip& owner);
        juce::ThreadPoolJob::JobStatus runJob() override;
    private:
        FilmStrip& owner;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThumbnailJob)
    };
private:
    /** @Internal */
    void update();

    /** @Internal */
    void setThumbnail (int index, juce::Image image);

    /** @Internal */
    juce::ThreadPool* getThreadPool();

    std::shared_ptr<AVClip> clip;
    double startTime = {};
    double timeLength = {};
    double aspectRatio = 1.33;

    std::unique_ptr<ThumbnailJob> thumbnailJob;
    std::vector<juce::Image> thumbnails;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilmStrip)
};

}
