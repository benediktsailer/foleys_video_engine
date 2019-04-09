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

namespace foleys
{

VideoEngine::VideoEngine()
{
    for (int i = 0; i < juce::SystemStats::getNumCpus(); ++i)
        readingThreads.emplace_back (std::make_unique<juce::TimeSliceThread>("Reading Thread #" + juce::String (i)));

    for (auto& reader : readingThreads)
        reader->startThread();

    startTimer (1000);
}

VideoEngine::~VideoEngine()
{
    for (auto& reader : readingThreads)
        reader->stopThread (500);
}

std::shared_ptr<AVClip> VideoEngine::createClipFromFile (juce::File file)
{
    auto clip = AVFormatManager::createClipFromFile (file);
    if (clip)
        addToThreadPool (clip);

    return clip;
}

std::shared_ptr<AVCompoundClip> VideoEngine::createCompoundClip()
{
    auto clip = std::make_shared<AVCompoundClip>();
    addToThreadPool (clip);
    return clip;
}

void VideoEngine::addToThreadPool (std::shared_ptr<AVClip> clip)
{
    releasePool.push_back (clip);

    auto* client = clip->getBackgroundJob();
    if (client == nullptr)
        return;

    int minClients = std::numeric_limits<int>::max();
    int nextThread = 0;
    for (int i=0; i < readingThreads.size(); ++i)
    {
        auto& thread = readingThreads.at (i);
        if (thread->getNumClients() <= minClients)
        {
            minClients = thread->getNumClients();
            nextThread = i;
        }
    }
    readingThreads [nextThread]->addTimeSliceClient (client);
}

void VideoEngine::removeFromThreadPool (std::shared_ptr<AVClip> clip)
{
    if (auto* client = clip->getBackgroundJob())
        for (auto& reader : readingThreads)
            reader->removeTimeSliceClient (client);
}

void VideoEngine::addJob (std::function<void()> job)
{
    jobThreads.addJob (std::move (job));
}

void VideoEngine::addJob (juce::ThreadPoolJob* job, bool deleteJobWhenFinished)
{
    jobThreads.addJob (job, deleteJobWhenFinished);
}

void VideoEngine::cancelJob (juce::ThreadPoolJob* job)
{
    jobThreads.removeJob (job, true, 100);
}

juce::ThreadPool& VideoEngine::getThreadPool()
{
    return jobThreads;
}

void VideoEngine::timerCallback()
{
    for (auto p = releasePool.begin(); p != releasePool.end();)
    {
        if (p->use_count() == 1)
        {
            removeFromThreadPool (*p);
            p = releasePool.erase (p);
        }
        else
        {
            ++p;
        }
    }
}


} // foleys