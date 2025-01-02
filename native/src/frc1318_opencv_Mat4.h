/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class frc1318_opencv_Mat4 */

#ifndef _Included_frc1318_opencv_Mat4
#define _Included_frc1318_opencv_Mat4
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     frc1318_opencv_Mat4
 * Method:    extractYawPitchRoll
 * Signature: (J[DIII)V
 */
JNIEXPORT void JNICALL Java_frc1318_opencv_Mat4_extractYawPitchRoll
  (JNIEnv *, jclass, jlong, jdoubleArray, jint, jint, jint);

/*
 * Class:     frc1318_opencv_Mat4
 * Method:    extractOffsetsAndAngles
 * Signature: (J[D[DIII)V
 */
JNIEXPORT void JNICALL Java_frc1318_opencv_Mat4_extractOffsetsAndAngles
  (JNIEnv *, jclass, jlong, jdoubleArray, jdoubleArray, jint, jint, jint);

/*
 * Class:     frc1318_opencv_Mat4
 * Method:    createAff
 * Signature: (DDDDDDI)J
 */
JNIEXPORT jlong JNICALL Java_frc1318_opencv_Mat4_createAff
  (JNIEnv *, jclass, jdouble, jdouble, jdouble, jdouble, jdouble, jdouble, jint);

/*
 * Class:     frc1318_opencv_Mat4
 * Method:    inv
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_frc1318_opencv_Mat4_inv
  (JNIEnv *, jclass, jlong);

/*
 * Class:     frc1318_opencv_Mat4
 * Method:    invAff
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_frc1318_opencv_Mat4_invAff
  (JNIEnv *, jclass, jlong);

/*
 * Class:     frc1318_opencv_Mat4
 * Method:    mult
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_frc1318_opencv_Mat4_mult
  (JNIEnv *, jclass, jlong, jlong);

/*
 * Class:     frc1318_opencv_Mat4
 * Method:    delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_frc1318_opencv_Mat4_delete
  (JNIEnv *, jclass, jlong);

/*
 * Class:     frc1318_opencv_Mat4
 * Method:    extractValues
 * Signature: (J)[D
 */
JNIEXPORT jdoubleArray JNICALL Java_frc1318_opencv_Mat4_extractValues
  (JNIEnv *, jclass, jlong);

#ifdef __cplusplus
}
#endif
#endif