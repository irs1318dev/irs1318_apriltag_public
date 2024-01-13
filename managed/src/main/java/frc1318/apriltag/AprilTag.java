package frc1318.apriltag;

public class AprilTag
{
    static {
        System.loadLibrary("irs1318_apriltag_native");
    }

    /**
     * Create an AprilTags detector object
     * @param family which family of tags to detect?
     * @param hammingDistance Detect tags with up to this many bit errors?
     * @param nThreads How many threads should be used?
     * @param quadDecimate detection of quads can be done on a lower-resolution image, improving speed at a cost of pose accuracy and a slight decrease in detection rate. Decoding the binary payload is still done at full resolution.
     * @param quadSigma What Gaussian blur should be applied to the segmented image (used for quad detection?)  Parameter is the standard deviation in pixels.  Very noisy images benefit from non-zero values (e.g. 0.8).
     * @param refineEdges whether the edges of the each quad are adjusted to "snap to" strong gradients nearby. This is useful when decimation is employed, as it can increase the quality of the initial quad estimate substantially. Generally recommended to be on (true). Very computationally inexpensive. Option is ignored if quad_decimate = 1.
     * @param decodeSharpening How much sharpening should be done to decoded images? This can help decode small tags but may or may not help in odd lighting conditions or low light conditions. The default value is 0.25.
     * @param debug whether to write a variety of debugging images to the current working directory at various stages through the detection process. (Somewhat slow).
     * @return Created detector object
     */
    public static AprilTagDetector create(
        AprilTagFamily family,
        int hammingDistance,
        int nThreads,
        float quadDecimate,
        float quadSigma,
        boolean refineEdges,
        double decodeSharpening,
        boolean debug)
    {
        return new AprilTagDetector(AprilTag.createDetector(family.value, hammingDistance, nThreads, quadDecimate, quadSigma, refineEdges, decodeSharpening, debug));
    }

    private static native long createDetector(
        int family,
        int hammingDistance,
        int nThreads,
        float quadDecimate,
        float quadSigma,
        boolean refineEdges,
        double decodeSharpening,
        boolean debug);
}
