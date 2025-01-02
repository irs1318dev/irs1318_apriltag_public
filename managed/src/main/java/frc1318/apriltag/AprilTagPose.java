package frc1318.apriltag;

import frc1318.opencv.Mat4;

/**
 * AprilTag Pose estimate
 */
public class AprilTagPose
{
    private Mat4 affineTransformation;
    private final double error;

    public AprilTagPose(Mat4 affineTransformation, double error)
    {
        this.affineTransformation = affineTransformation;
        this.error = error;
    }

    public double getError()
    {
        return this.error;
    }

    public Mat4 getTransformation()
    {
        return this.affineTransformation;
    }

    public void release()
    {
        if (this.affineTransformation != null)
        {
            this.affineTransformation.release();
            this.affineTransformation = null;
        }
    }
}
