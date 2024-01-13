package frc1318.apriltag;

import org.opencv.core.Point;

public class AprilTagDetection
{
    // The native object that is associated with this detection result
    private final long nativeObj;

    // The decoded ID of the tag
    private final int id;

    // How many error bits were corrected? Note: accepting large numbers of
    // corrected errors leads to greatly increased false positive rates.
    // NOTE: As of this implementation, the detector cannot detect tags with
    // a hamming distance greater than 2.
    private final int hamming;

    // A measure of the quality of the binary decoding process: the
    // average difference between the intensity of a data bit versus
    // the decision threshold. Higher numbers roughly indicate better
    // decodes. This is a reasonable measure of detection accuracy
    // only for very small tags-- not effective for larger tags (where
    // we could have sampled anywhere within a bit cell and still
    // gotten a good detection.)
    private final float decisionMargin;

    // The center of the detection in image pixel coordinates.
    private final Point c;

    // The 3x3 homography matrix describing the projection from an
    // "ideal" tag (with corners at (-1,1), (1,1), (1,-1), and (-1,
    // -1)) to pixels in the image. This matrix will be freed by
    // apriltag_detection_destroy.
    // Stored as a matrix structure for holding double-precision values with
    //  data in row-major order (i.e. index = row*ncols + col).
    private double[] h;

    // The four corners of the tag in image pixel coordinates. These always
    // wrap counter-clock wise around the tag.
    private Point[] p;

    public AprilTagDetection(long nativeObj, int id, int hamming, float decisionMargin, Point c)
    {
        this.nativeObj = nativeObj;
        this.id = id;
        this.hamming = hamming;
        this.decisionMargin = decisionMargin;
        this.c = c;
    }

    /**
     * Gets the Id of the tag
     * @return the decoded ID of the tag
     */
    public int getId()
    {
        return this.id;
    }

    /**
     * Gets the hamming distance
     * @return how many error bits were corrected? Note: accepting large numbers of corrected errors leads to greatly increased false positive rates. NOTE: As of this implementation, the detector cannot detect tags with a hamming distance greater than 2.
     */
    public int getHamming()
    {
        return this.hamming;
    }

    /**
     * Gets a measure of the quality of the binary decoding process
     * @return the average difference between the intensity of a data bit versus the decision threshold. Higher numbers roughly indicate better decodes. This is a reasonable measure of detection accuracy only for very small tags-- not effective for larger tags (where we could have sampled anywhere within a bit cell and still gotten a good detection.)
     */
    public float getDecisionMargin()
    {
        return this.decisionMargin;
    }

    /**
     * Gets the center
     * @return the center of the detection in image pixel coordinates.
     */
    public Point getCenter()
    {
        return this.c;
    }

    /**
     * Get the Homography Matrix
     * @return The 3x3 homography matrix describing the projection from an "ideal" tag (with corners at (-1,1), (1,1), (1,-1), and (-1, -1)) to pixels in the image. Stored as a matrix structure for holding double-precision values with data in row-major order (i.e. index = row*ncols + col).
     */
    public double[] getHomographyMatrix()
    {
        if (this.h == null)
        {
            this.h = AprilTagDetection.getHomographyMatrix(this.nativeObj);
        }

        return this.h;
    }

    /**
     * Gets the vertices
     * @return the four corners of the tag in image pixel coordinates. These always wrap counter-clock wise around the tag.
     */
    public Point[] getVertices()
    {
        if (this.p == null)
        {
            this.p = (Point[])AprilTagDetection.getVertices(this.nativeObj);
        }

        return this.p;
    }

    /**
     * Estimates the tag's pose based on the detection and camera information
     * @param tagSize in inches
     * @param fx focal length (X)
     * @param fy focal length (Y)
     * @param cx camera center (X)
     * @param cy camera center (Y)
     * @param premultiplyMat affine transformatiomn matrix to pre-multiply
     * @param offsetResults the x, y, and z offsets from the robot, in inches
     * @param yprResults the yaw, pitch, and roll offsets from the robot, in degrees
     * @return affine tranformation matrix as an OpenCV matrix
     */
    public AprilTagPose estimateTagPose(
        double tagSize,
        double fx,
        double fy,
        double cx,
        double cy,
        Mat4 premultiplyMat,
        double[] offsetResults,
        double[] yprResults)
    {
        return
            (AprilTagPose)AprilTagDetection.estimateTagPose(
                this.nativeObj,
                tagSize,
                fx,
                fy,
                cx,
                cy,
                premultiplyMat.nativeObj,
                offsetResults,
                yprResults);
    }

    public void destroy()
    {
        AprilTagDetection.destroy(this.nativeObj);
    }

    private static native double[] getHomographyMatrix(long nativeObj);

    private static native Object[] getVertices(long nativeObj);

    private static native Object estimateTagPose(long nativeObj, double tagSize, double fx, double fy, double cx, double cy, long premultiplyMatObj, double[] offsetResults, double[] yprResults);

    private static native void destroy(long nativeObj);
}
