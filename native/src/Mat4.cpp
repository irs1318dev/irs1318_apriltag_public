#include <jni.h>
#include <opencv2/opencv.hpp>
#include "Mat4.hpp"

extern "C" /* specify the C calling convention */
{
    using namespace cv;

    /*
     * Class:     frc1318_apriltag_Mat4
     * Method:    extractYawPitchRoll
     * Signature: (J[DIII)V
     */
    JNIEXPORT void JNICALL Java_frc1318_apriltag_Mat4_extractYawPitchRoll
        (JNIEnv* env, jclass objClass, jlong mat4Obj, jdoubleArray ypr, jint type, jint form, jint ss)
    {
        Matx44d* mat = (Matx44d*)mat4Obj;
        double yprArray[3]{ 0.0 };
        extract_ypr(mat, yprArray[0], yprArray[1], yprArray[2], type, form, ss);
        yprArray[0] *= DEGREES_PER_RADIAN;
        yprArray[1] *= DEGREES_PER_RADIAN;
        yprArray[2] *= DEGREES_PER_RADIAN;
        env->SetDoubleArrayRegion(ypr, 0, 3, yprArray);
    }

    /*
     * Class:     frc1318_apriltag_Mat4
     * Method:    extractOffsetsAndAngles
     * Signature: (J[D[DIII)V
     */
    JNIEXPORT void JNICALL Java_frc1318_apriltag_Mat4_extractOffsetsAndAngles
        (JNIEnv* env, jclass objClass, jlong mat4Obj, jdoubleArray off, jdoubleArray ypr, jint type, jint form, jint ss)
    {
        Matx44d* mat = (Matx44d*)mat4Obj;

        double yprArray[3]{ 0.0 };
        extract_ypr(mat, yprArray[0], yprArray[1], yprArray[2], type, form, ss);
        yprArray[0] *= DEGREES_PER_RADIAN;
        yprArray[1] *= DEGREES_PER_RADIAN;
        yprArray[2] *= DEGREES_PER_RADIAN;
        env->SetDoubleArrayRegion(ypr, 0, 3, yprArray);

        double offArray[3]{ 0.0 };
        offArray[0] = mat->val[0 * 4 + 3];
        offArray[1] = mat->val[1 * 4 + 3];
        offArray[2] = mat->val[2 * 4 + 3];
        env->SetDoubleArrayRegion(off, 0, 3, offArray);
    }


    /*
     * Class:     frc1318_apriltag_Mat4
     * Method:    createAff
     * Signature: (DDDDDDI)J
     */
    JNIEXPORT jlong JNICALL Java_frc1318_apriltag_Mat4_createAff
        (JNIEnv* env, jclass objClass, jdouble yawDeg, jdouble pitchDeg, jdouble rollDeg, jdouble x, jdouble y, jdouble z, jint form)
    {
        return (jlong)createAff(yawDeg, pitchDeg, rollDeg, x, y, z, form, true);
    }

    /*
     * Class:     frc1318_apriltag_Mat4
     * Method:    inv
     * Signature: (J)J
     */
    JNIEXPORT jlong JNICALL Java_frc1318_apriltag_Mat4_inv
        (JNIEnv* env, jclass objClass, jlong mat4Obj)
    {
        Matx44d* mat = (Matx44d*)mat4Obj;
        Matx44d* result = new Matx44d(mat->inv());
        return (jlong)result;
    }

    /*
     * Class:     frc1318_apriltag_Mat4
     * Method:    invAff
     * Signature: (J)J
     */
    JNIEXPORT jlong JNICALL Java_frc1318_apriltag_Mat4_invAff
        (JNIEnv* env, jclass objClass, jlong mat4Obj)
    {
        Matx44d* mat = (Matx44d*)mat4Obj;

        Matx44d* result = invAff(mat);
        return (jlong)result;
    }

    /*
     * Class:     frc1318_apriltag_Mat4
     * Method:    mult
     * Signature: (JJ)J
     */
    JNIEXPORT jlong JNICALL Java_frc1318_apriltag_Mat4_mult
        (JNIEnv* env, jclass objClass, jlong mat4Obj1, jlong mat4Obj2)
    {
        Matx44d* mat1 = (Matx44d*)mat4Obj1;
        Matx44d* mat2 = (Matx44d*)mat4Obj2;
        Matx44d* result = new Matx44d((*mat1) * (*mat2));
        return (jlong)result;
    }

    /*
     * Class:     frc1318_apriltag_Mat4
     * Method:    delete
     * Signature: (J)V
     */
    JNIEXPORT void JNICALL Java_frc1318_apriltag_Mat4_delete
        (JNIEnv* env, jclass objClass, jlong mat4Obj)
    {
        Matx44d* mat = (Matx44d*)mat4Obj;
        delete mat;
    }

    /*
     * Class:     frc1318_apriltag_Mat4
     * Method:    extractValues
     * Signature: (J)[D
     */
    JNIEXPORT jdoubleArray JNICALL Java_frc1318_apriltag_Mat4_extractValues
        (JNIEnv* env, jclass objClass, jlong mat4Obj)
    {
        Matx44d* mat = (Matx44d*)mat4Obj;

        jdoubleArray values = env->NewDoubleArray(16);
        env->SetDoubleArrayRegion(values, 0, 16, mat->val);
        return values;
    }
}
