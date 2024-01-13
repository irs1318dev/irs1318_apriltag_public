package frc1318.apriltag;

/**
 * AprilTag Family Values need to be kept in sync with native code.
 */
public enum AprilTagFamily
{
    tag36h11(0),
    tag25h9(1),
    tag36h10(2),
    tag16h5(3),
    tagCircle21h7(4),
    tagCircle49h12(5),
    tagCustom48h12(6),
    tagStandard41h12(7),
    tagStandard52h13(8);

    public final int value;
    private AprilTagFamily(int value)
    {
        this.value = value;
    }
}
