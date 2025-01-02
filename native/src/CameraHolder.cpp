#ifdef UseLibCamera

#include <jni.h>
#include <opencv2/opencv.hpp>
#include <libcamera/libcamera.h>
#include <sys/mman.h>
#include "CameraHolder.hpp"

using namespace cv;
using namespace libcamera;

int CameraHolder::configure(int resX, int resY, int frameRate, ImageFormat format)
{
    // generate default camera configuration based on information advertised by the camera
    std::unique_ptr<CameraConfiguration> config = this->camera->generateConfiguration({ StreamRole::VideoRecording });

    // Convert from frameRate to microseconds
    this->frameDurationLimits[0] =  1000000 / frameRate;
    this->frameDurationLimits[1] =  1000000 / frameRate;

    ////std::cout << "config: " << config->orientation << ", sensorConfig.has_value(): " << (config->sensorConfig.has_value() ? "true" : "false") << std::endl;

    // update the camera configuration to our desired resolution and pixel format
    StreamConfiguration &streamConfig = config->at(0);
    streamConfig.size = libcamera::Size(resX, resY);
    switch (format)
    {
        case ImageFormat::Gray:
        default:
            streamConfig.pixelFormat = formats::YUV420;
            break;
    }

    ////std::cout << "validating: resX " << streamConfig.size.width << " resY " << streamConfig.size.height << " stride " << streamConfig.stride << " pixelFormat " << streamConfig.pixelFormat << " colorSpace " << streamConfig.colorSpace->toString() << std::endl;

    // validate config
    CameraConfiguration::Status validateResult = config->validate();
    if (validateResult == CameraConfiguration::Status::Invalid)
    {
        std::cerr << "Couldn't validate camera config: " << validateResult << std::endl;
        return -1;
    }

    ////std::cout << "validated (" << validateResult << "): resX " << streamConfig.size.width << " resY " << streamConfig.size.height << " stride " << streamConfig.stride << " pixelFormat " << streamConfig.pixelFormat << " colorSpace " << streamConfig.colorSpace->toString() << std::endl;
    ////std::cout << "config: " << config->orientation << ", sensorConfig.has_value(): " << (config->sensorConfig.has_value() ? "true" : "false") << std::endl;

    // apply config
    int configureResult = this->camera->configure(config.get());
    if (configureResult != 0)
    {
        std::cerr << "Couldn't configure camera: " << configureResult << std::endl;
        return configureResult;
    }

    ////std::cout << "configured (" << configureResult << "): resX " << streamConfig.size.width << " resY " << streamConfig.size.height << " stride " << streamConfig.stride << " pixelFormat " << streamConfig.pixelFormat << " colorSpace " << streamConfig.colorSpace->toString() << std::endl;

    // create frame buffer allocator, grab reference to stream
    this->allocator = new FrameBufferAllocator(this->camera);
    this->stream = streamConfig.stream();

    // allocate stream buffers
    int allocateResult = this->allocator->allocate(this->stream);
    if (allocateResult < 0)
    {
        std::cerr << "Couldn't allocate stream buffers: " << allocateResult << std::endl;

        delete this->allocator;
        this->allocator = nullptr;

        this->stream = nullptr;

        return allocateResult;
    }

    // prepare the set of requests
    const std::vector<std::unique_ptr<FrameBuffer>> &buffers = this->allocator->buffers(this->stream);
    this->requests.clear();
    this->requests.reserve(buffers.size());
    for (unsigned int i = 0; i < buffers.size(); i++)
    {
        std::unique_ptr<Request> request = this->camera->createRequest();
        if (request == nullptr)
        {
            std::cerr << "Couldn't create request " << i << " for camera" << std::endl;
            return -1;
        }

        const std::unique_ptr<FrameBuffer> &buffer = buffers[i];
        int addBufferResult = request->addBuffer(this->stream, buffer.get());
        if (addBufferResult != 0)
        {
            std::cerr << "Couldn't add buffer " << i << " to request for camera: " << addBufferResult << std::endl;
            return addBufferResult;
        }

        // Now we memory-map the various buffers that were allocated by FrameBufferAllocator so that we can copy the data into OpenCV Mats later.

        ////std::cout << "buffer has " << buffer->planes().size() << " planes" << std::endl;
        int buffersToMap;
        switch (format)
        {
            ////case Something:
            ////    buffersToMap = buffer->planes().size();
            ////    break;

            case ImageFormat::Gray:
            default:
                buffersToMap = 1;
                break;
        }

        size_t bufferSize = 0;
        for (int i = 0; i < buffersToMap; i++)
        {
            // a buffer could appear to be spread across multiple planes, but only actually use the same file descriptor
            // in that case, we can accumulate them to only mmap once
            const libcamera::FrameBuffer::Plane& plane = buffer->planes()[i];
            bufferSize += plane.length;
            if (i == buffersToMap - 1 // if this is the last plane we will be mapping
                || plane.fd.get() != buffer->planes()[i + 1].fd.get()) // or the next plane uses a different fd
            {
                void *memory = mmap(nullptr, bufferSize, PROT_READ | PROT_WRITE, MAP_SHARED, plane.fd.get(), 0);
                this->mmappedBuffers[buffer.get()].push_back(libcamera::Span<uint8_t>(static_cast<uint8_t*>(memory), bufferSize));
                bufferSize = 0;
            }
        }

        // set initial controls, including frame rate:
        ControlList &controls = request->controls();
        controls.set(controls::FrameDurationLimits, libcamera::Span<const std::int64_t, 2UL>(this->frameDurationLimits));
        controls.set(controls::Brightness, this->brightness);
        controls.set(controls::ExposureValue, this->exposure);

        this->requests.push_back(std::move(request));
    }

    // set the callback for completed requests
    this->camera->requestCompleted.connect(this, &CameraHolder::requestComplete);

    return 0;
}

int CameraHolder::startCapturing()
{
    // transition the camera into running state
    int startResult = this->camera->start();
    if (startResult != 0)
    {
        std::cerr << "Couldn't start camera: " << startResult << std::endl;
        return startResult;
    }

    // add requests to queue
    for (std::unique_ptr<Request> &request : this->requests)
    {
        int queueResult = this->camera->queueRequest(request.get());
        if (queueResult != 0)
        {
            std::cerr << "Couldn't queue request: " << queueResult << std::endl;
            return queueResult;
        }
    }

    return 0;
}

void CameraHolder::requestComplete(Request* request)
{
    if (request == nullptr)
    {
        return;
    }

    if (request->status() == Request::RequestCancelled)
    {
        return;
    }

    const Request::BufferMap& buffers = request->buffers();
    for (auto bufferPair : buffers)
    {
        std::vector<Span<uint8_t>> memoryBlocks = this->getMmappedBuffers(bufferPair.second);
        const StreamConfiguration& streamConfig = bufferPair.first->configuration();
        unsigned int resX = streamConfig.size.width;
        unsigned int resY = streamConfig.size.height;
        unsigned int stride = streamConfig.stride;

        Mat* frame;
        ////Mat* frame = new Mat(resY, resX, CV_8UC1, mem[0].data(), stride);
        switch (this->configuredFormat)
        {
            case ImageFormat::Gray:
            default:
                // we only use/copy the first channel data (Y, from YUV420) for Grayscale
                frame = new Mat(resY, resX, CV_8UC1);
                uint8_t *ptr = memoryBlocks[0].data();
                for (unsigned int i = 0; i < resY; i++, ptr += stride)
                {
                    memcpy(frame->ptr(i), ptr, stride);
                }

                break;
        }

        this->swapFrame(frame);
    }

    if (request->status() == Request::RequestCancelled)
    {
    	return;
    }

    request->reuse(Request::ReuseBuffers);

    this->settingsLock.lock();
    if (this->applySettings.at(request))
    {
        ControlList &controls = request->controls();
        controls.set(controls::Brightness, this->brightness);
        controls.set(controls::ExposureValue, this->exposure);
    }

    this->settingsLock.unlock();

    this->camera->queueRequest(request);
}

void CameraHolder::updateSettings(float brightness, float exposure)
{
    this->settingsLock.lock();
    for (std::unique_ptr<Request> &request : this->requests)
    {
        this->applySettings[request.get()] = true;
    }

    this->brightness = brightness;
    this->exposure = exposure;

    this->settingsLock.unlock();
}

void CameraHolder::stopCapturing()
{
    this->camera->stop();
}

std::string CameraHolder::getControls() const
{
    std::string result = "";
    const ControlInfoMap& controls = this->camera->controls();
    for (auto control : controls)
    {
        result += control.first->name();
        bool first = true;
        for (auto controlValue : control.second.values())
        {
            if (!first)
            {
                result += ", ";
            }
            else
            {
                result += ": ";
                first = false;
            }

            result += controlValue.toString();
        }

        result += "\n";
    }

    return result;
}

Mat* CameraHolder::read()
{
    this->frameLock.lock();

    // grab the most recent frame to return
    Mat* result = this->frame;
    this->frame = nullptr;

    this->frameLock.unlock();

    return result;
}

void CameraHolder::release()
{
    this->camera->requestCompleted.disconnect(this, &CameraHolder::requestComplete);

    // try to unmap the mmapped buffers
    for (auto bufferMapping : this->mmappedBuffers)
    {
        for (auto mmapedRegions : bufferMapping.second)
        {
            munmap(static_cast<void*>(mmapedRegions.data()), mmapedRegions.size());
        }

        bufferMapping.second.clear();
    }

    this->mmappedBuffers.clear();

    this->camera->release();
}

std::vector<libcamera::Span<uint8_t>> CameraHolder::getMmappedBuffers(libcamera::FrameBuffer *buffer) const
{
    auto item = this->mmappedBuffers.find(buffer);
    if (item == this->mmappedBuffers.end())
    {
        return {};
    }

    return item->second;
}

void CameraHolder::swapFrame(cv::Mat* newFrame)
{
    this->frameLock.lock();

    if (this->frame != nullptr)
    {
        delete this->frame;
    }

    this->frame = newFrame;

    this->frameLock.unlock();
}

#endif