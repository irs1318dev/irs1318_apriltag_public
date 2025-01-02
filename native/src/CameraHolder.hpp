#ifdef UseLibCamera

#include <opencv2/opencv.hpp>
#include <libcamera/libcamera.h>

enum class ImageFormat : int
{
    Gray = 0,
};

// a simple holder class to hold the shared_ptr to the camera to ensure it doesn't go out of scope and get released when control goes back to the Java side...
// Some pieces (mmap buffering) inspired by the very helpful repository here: https://github.com/berndporr/libcamera2opencv.
class CameraHolder
{
private:
    std::shared_ptr<libcamera::Camera> camera;
    libcamera::FrameBufferAllocator* allocator;
    libcamera::Stream* stream;
    std::vector<std::unique_ptr<libcamera::Request>> requests;
    std::map<libcamera::FrameBuffer*, std::vector<libcamera::Span<uint8_t>>> mmappedBuffers;

    ImageFormat configuredFormat;
    std::int64_t frameDurationLimits[2]; // frame rates in microseconds, where [0] is a minimum duration, and [1] is a maximum

    cv::Mat* frame;
    std::mutex frameLock;

    std::map<libcamera::Request*, bool> applySettings;
    float brightness;
    float exposure;
    std::mutex settingsLock;

    void requestComplete(libcamera::Request *request);
    std::vector<libcamera::Span<uint8_t>> getMmappedBuffers(libcamera::FrameBuffer *buffer) const;
    void swapFrame(cv::Mat* newFrame);

public:
    CameraHolder(std::shared_ptr<libcamera::Camera> value) :
        camera(value),
        allocator(nullptr),
        stream(nullptr),
        requests(),
        mmappedBuffers(),
        frame(nullptr),
        frameLock(),
        applySettings(),
        brightness(),
        exposure(),
        settingsLock(),
        configuredFormat(ImageFormat::Gray),
        frameDurationLimits()
    {
    }

    std::string getControls() const;
    int configure(int resX, int resY, int frameRate, ImageFormat format);
    void updateSettings(float brightness, float exposure);
    int startCapturing();
    void stopCapturing();
    cv::Mat* read();
    void release();
};

#endif