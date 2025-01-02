package frc1318.opencv;

import frc1318.ReleaseableBase;

/**
 * Wrapper for OpenCV's C++ 4x4 Matrix of doubles (Matx44d)
 */
public class Mat4 extends ReleaseableBase
{
    public final long nativeObj;

    public Mat4(long nativeObj)
    {
        super();

        this.nativeObj = nativeObj;
    }

    /**
     * Create an affine transformation matrix with the provided angle rotations and translation
     * @param yaw angle in degrees
     * @param pitch angle in degrees
     * @param roll angle in degrees
     * @param x distance in inches
     * @param y distance in inches
     * @param z distance in inches
     * @param form of rotation matrix to create: ZYX (0), XYZ (1), ...
     * @return affine transformation matrix
     */
    public static Mat4 createAffine(double yaw, double pitch, double roll, double x, double y, double z, int form)
    {
        return new Mat4(Mat4.createAff(yaw, pitch, roll, x, y, z, form));
    }

    /**
     * Invert this matrix
     * @return inverted matrix (new object)
     */
    public Mat4 invert()
    {
        this.checkReleased();

        return new Mat4(Mat4.inv(this.nativeObj));
    }

    /**
     * Invert this matrix, assuming it is an affine transformation matrix
     * @return inverted matrix (new object)
     */
    public Mat4 invertAffine()
    {
        this.checkReleased();

        return new Mat4(Mat4.invAff(this.nativeObj));
    }

    /**
     * Multiply this matrix by the other matrix
     * @param other other matrix to multiply (after) this matrix
     * @return result of multiplication (new object)
     */
    public Mat4 multiply(Mat4 other)
    {
        this.checkReleased();

        return new Mat4(Mat4.mult(this.nativeObj, other.nativeObj));
    }

    /**
     * Assuming this is an affine transformation matrix
     * Extract yaw, pitch, and roll from the 4x4 Affine Transformation (or 3x3 Rotation) matrix
     * @param matrix to extract from
     * @param ypr yaw, pitch, and roll values (in degrees)
     * @param type whether to extract ypr as yaw, pitch, roll or roll, pitch, yaw
     * @param form of rotation matrix to extract from: ZYX (0), XYZ (1), ...
     * @param ss sqrt-sign -1/+1
     */
    public void extractYawPitchRoll(double[] ypr, int type, int form, int ss)
    {
        this.checkReleased();

        Mat4.extractYawPitchRoll(this.nativeObj, ypr, type, form, ss);
    }

    /**
     * Assuming this is a 4x4 Affine Transformation matrix,
     * Extract offsets and angles.
     * @param offsets x, y, and z translation offsets (in inches)
     * @param ypr yaw, pitch, and roll rotation angles (in degrees)
     * @param type whether to extract ypr as yaw, pitch, roll or roll, pitch, yaw
     * @param form of rotation matrix to extract from: ZYX (0), XYZ (1), ...
     * @param ss sqrt-sign -1/+1
     */
    public void extractOffsetsAndAngles(double[] offsets, double[] ypr, int type, int form, int ss)
    {
        this.checkReleased();

        Mat4.extractOffsetsAndAngles(this.nativeObj, offsets, ypr, type, form, ss);
    }
    
    /**
     * Convert the provided matrix into a string
     * @param matrix to print
     * @return string representing the matrix
     */
    @Override
    public String toString()
    {
        this.checkReleased();

        double[] values = Mat4.extractValues(this.nativeObj);

        StringBuilder builder = new StringBuilder();
        for (int row = 0; row < 4; row++)
        {
            builder.append("\n");

            builder.append('[');
            for (int col = 0; col < 4; col++)
            {
                if (col > 0)
                {
                    builder.append(" ");
                }

                if (values != null && values.length > 0)
                {
                    builder.append(String.format("%.2f", values[row * 4 + col]));
                }
            }

            builder.append("]");
        }

        return builder.toString();
    }

    /**
     * Release native memory
     */
    @Override
    protected void releaseInternal()
    {
        Mat4.delete(this.nativeObj);
    }

    private static native long createAff(double yaw, double pitch, double roll, double x, double y, double z, int type);
    private static native void extractYawPitchRoll(long nativeObj, double[] ypr, int type, int form, int ss);
    private static native void extractOffsetsAndAngles(long nativeObj, double[] offsets, double[] ypr, int type, int form, int ss);
    private static native long inv(long nativeObj);
    private static native long invAff(long nativeObj);
    private static native long mult(long nativeObj1, long nativeObj2);
    private static native void delete(long nativeObj);
    private static native double[] extractValues(long nativeObj);
}
