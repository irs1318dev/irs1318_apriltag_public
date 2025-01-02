#include <opencv2/opencv.hpp>

using namespace cv;

const double PI = 3.14159265358979323846264;
const double RADIANS_PER_DEGREE = PI / 180.0;
const double DEGREES_PER_RADIAN = 180.0 / PI;

// extract yaw, pitch, and roll from from a 4x4 Affine Transformation (or 3x3 Rotation) matrix 
// ss is the sqrt solution form (-1 or 1)
// results are in radians
template<int n>
static void extract_ypr(Matx<double, n, n>* m, double& yaw, double& pitch, double& roll, int type, int form, int ss)
{
    static_assert(n == 3 || n == 4, "Matx should have 3 or 4 rows+columns.");

    Matx<double, n, n> mat = *m;
    if (form == 0)
    {
        // GBA mode
        if (type == 0)
        {
            yaw = atan2(
                mat(0, 1),
                mat(0, 0));
            pitch = atan2(
                -mat(0, 2),
                (double)ss * sqrt(mat(0, 0) * mat(0, 0) + mat(0, 1) * mat(0, 1)));
            roll = atan2(
                mat(1, 2),
                mat(2, 2));
        }
        else //if (type == 1)
        {
            roll = atan2(
                mat(0, 1),
                mat(0, 0));
            pitch = atan2(
                -mat(0, 2),
                (double)ss * sqrt(mat(0, 0) * mat(0, 0) + mat(0, 1) * mat(0, 1)));
            yaw = atan2(
                mat(1, 2),
                mat(2, 2));
        }
    }
    else //if (form == 1)
    {
        // ABG mode - Tait–Bryan angles X1 Y2 Z3
        if (type == 0)
        {
            yaw = atan2(
                -mat(0, 1),
                mat(0, 0));
            pitch = atan2(
                mat(0, 2),
                (double)ss * sqrt(mat(0, 0) * mat(0, 0) + mat(0, 1) * mat(0, 1)));
            roll = atan2(
                -mat(1, 2),
                mat(2, 2));
        }
        else //if (type == 1)
        {
            roll = atan2(
                -mat(0, 1),
                mat(0, 0));
            pitch = atan2(
                mat(0, 2),
                (double)ss * sqrt(mat(0, 0) * mat(0, 0) + mat(0, 1) * mat(0, 1)));
            yaw = atan2(
                -mat(1, 2),
                mat(2, 2));
        }
    }
}

// invert the affine matrix using the shortcut method (transpose rotation, multiply translation by negative transposed rotation)
static Matx44d* invAff(Matx44d* m)
{
    Matx44d mat = *m;

    // create already-inverted rotation matrix:
    Matx33d newRotationMatrix(
        mat(0, 0), mat(1, 0), mat(2, 0),
        mat(0, 1), mat(1, 1), mat(2, 1),
        mat(0, 2), mat(1, 2), mat(2, 2));

    Vec3d origTranslation(mat(0, 3), mat(1, 3), mat(2, 3));
    Vec3d newTranslation = -newRotationMatrix * origTranslation;

    Matx44d* result = new Matx44d(
        newRotationMatrix(0, 0), newRotationMatrix(0, 1), newRotationMatrix(0, 2), newTranslation(0),
        newRotationMatrix(1, 0), newRotationMatrix(1, 1), newRotationMatrix(1, 2), newTranslation(1),
        newRotationMatrix(2, 0), newRotationMatrix(2, 1), newRotationMatrix(2, 2), newTranslation(2),
        0.0, 0.0, 0.0, 1.0);

    return result;
}

static Matx44d* createAff(double yaw, double pitch, double roll, double x, double y, double z, int form, bool degrees)
{
    double yawRad = yaw;
    double pitchRad = pitch;
    double rollRad = roll;
    if (degrees)
    {
        yawRad = yaw * RADIANS_PER_DEGREE;
        pitchRad = pitch * RADIANS_PER_DEGREE;
        rollRad = roll * RADIANS_PER_DEGREE;
    }

    double cosYaw = cos(yawRad);
    double sinYaw = sin(yawRad);
    double cosPitch = cos(pitchRad);
    double sinPitch = sin(pitchRad);
    double cosRoll = cos(rollRad);
    double sinRoll = sin(rollRad);

    Matx44d* result;
    if (form == 0)
    {
        // GBA (ZYX) mode
        result = new Matx44d(
            cosYaw * cosPitch,
            cosYaw * sinPitch * sinRoll - sinYaw * cosRoll,
            cosYaw * sinPitch * cosRoll + sinYaw * sinRoll,
            x,

            sinYaw * cosPitch,
            sinYaw * sinPitch * sinRoll + cosYaw * cosRoll,
            sinYaw * sinPitch * cosRoll - cosYaw * sinRoll,
            y,

            -sinPitch,
            cosPitch * sinRoll,
            cosPitch * cosRoll,
            z,

            0.0,
            0.0,
            0.0,
            1.0);
    }
    else //if (form == 1)
    {
        // ABG (XYZ) mode
        result = new Matx44d(
            cosPitch * cosYaw,
            -cosPitch * sinYaw,
            sinPitch,
            x,

            sinRoll * sinPitch * cosYaw + cosRoll * sinYaw,
            cosRoll * cosYaw - sinRoll * sinPitch * sinYaw,
            -sinRoll * cosPitch,
            y,

            sinRoll * sinYaw - cosRoll * sinPitch * cosYaw,
            cosRoll * sinPitch * sinYaw + sinRoll * cosYaw,
            cosRoll * cosPitch,
            z,

            0.0,
            0.0,
            0.0,
            1.0);
    }

    return result;
}
