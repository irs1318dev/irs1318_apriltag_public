package frc1318.apriltag;

import org.opencv.core.Mat;

import frc1318.ReleaseableBase;

public class AprilTagDetector extends ReleaseableBase
{
    private final long nativeObj;

    AprilTagDetector(long nativeObj)
    {
        super();

        this.nativeObj = nativeObj;
    }

    public AprilTagDetection[] detect(Mat image)
    {
        this.checkReleased();

        return (AprilTagDetection[])AprilTagDetector.detect(this.nativeObj, image.nativeObj);
    }

    @Override
    protected void releaseInternal()
    {
        AprilTagDetector.release(this.nativeObj);
    }

    private static native Object[] detect(long nativeObj, long image);
    private static native void release(long nativeObj);
}
