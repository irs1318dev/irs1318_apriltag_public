package frc1318.libcamera;

import org.opencv.core.Mat;

import frc1318.ReleaseableBase;

public class Camera extends ReleaseableBase
{
    private final long nativeObj;

    private int resolutionX;
    private int resolutionY;
    private ImageFormat format;

    Camera(long nativeObj)
    {
        super();

        this.nativeObj = nativeObj;
    }

    public int getResolutionX()
    {
        return this.resolutionX;
    }

    public int getResolutionY()
    {
        return this.resolutionY;
    }

    public ImageFormat getPixelFormat()
    {
        return this.format;
    }

    public boolean configure(int resolutionX, int resolutionY, int frameRate, ImageFormat format)
    {
        int result = Camera.configure(this.nativeObj, resolutionX, resolutionY, frameRate, format.value);
        if (result == 0)
        {
            this.resolutionX = resolutionX;
            this.resolutionY = resolutionY;
            this.format = format;

            return true;
        }

        System.err.println("couldn't configure camera: " + result);
        return false;
    }

    public void updateSettings(float brightness, float exposure)
    {
        Camera.updateSettings(this.nativeObj, brightness, exposure);
    }

    public boolean start()
    {
        this.checkReleased();

        int result = Camera.startCapturing(this.nativeObj);
        if (result == 0)
        {
            return true;
        }

        System.err.println("couldn't start camera: " + result);
        return false;
    }

    public void stop()
    {
        this.checkReleased();

        Camera.stopCapturing(this.nativeObj);
    }

    public String getControls()
    {
        return Camera.getControls(this.nativeObj);
    }

    public Mat read()
    {
        this.checkReleased();

        return Camera.read(this.nativeObj);
    }

    @Override
    protected void releaseInternal()
    {
        Camera.delete(this.nativeObj);
    }

    private static native int configure(long nativeObj, int resX, int resY, int frameRate, int format);
    private static native void updateSettings(long nativeObj, float brightness, float exposure);
    private static native int startCapturing(long nativeObj);
    private static native void stopCapturing(long nativeObj);
    private static native String getControls(long nativeObj);
    private static native Mat read(long nativeObj);
    private static native void delete(long nativeObj);
}
