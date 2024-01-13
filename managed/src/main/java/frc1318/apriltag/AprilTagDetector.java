package frc1318.apriltag;

import org.opencv.core.Mat;

public class AprilTagDetector
{
    private long nativeObj;

    AprilTagDetector(long nativeObj)
    {
        this.nativeObj = nativeObj;
    }

    public AprilTagDetection[] detect(Mat image)
    {
        return (AprilTagDetection[])AprilTagDetector.detect(this.nativeObj, image.nativeObj);
    }

    public void destroy()
    {
        AprilTagDetector.destroy(this.nativeObj);
        this.nativeObj = 0;
    }

    private static native Object[] detect(long nativeObj, long image);

    private static native void destroy(long nativeObj);
}
