/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class frc1318_apriltag_AprilTagDetection */

#ifndef _Included_frc1318_apriltag_AprilTagDetection
#define _Included_frc1318_apriltag_AprilTagDetection
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     frc1318_apriltag_AprilTagDetection
 * Method:    getHomographyMatrix
 * Signature: (J)[D
 */
JNIEXPORT jdoubleArray JNICALL Java_frc1318_apriltag_AprilTagDetection_getHomographyMatrix
  (JNIEnv *, jclass, jlong);

/*
 * Class:     frc1318_apriltag_AprilTagDetection
 * Method:    getVertices
 * Signature: (J)[Ljava/lang/Object;
 */
JNIEXPORT jobjectArray JNICALL Java_frc1318_apriltag_AprilTagDetection_getVertices
  (JNIEnv *, jclass, jlong);

/*
 * Class:     frc1318_apriltag_AprilTagDetection
 * Method:    estimateTagPose
 * Signature: (JDDDDDJ[D[D)Ljava/lang/Object;
 */
JNIEXPORT jobject JNICALL Java_frc1318_apriltag_AprilTagDetection_estimateTagPose
  (JNIEnv *, jclass, jlong, jdouble, jdouble, jdouble, jdouble, jdouble, jlong, jdoubleArray, jdoubleArray);

/*
 * Class:     frc1318_apriltag_AprilTagDetection
 * Method:    destroy
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_frc1318_apriltag_AprilTagDetection_destroy
  (JNIEnv *, jclass, jlong);

#ifdef __cplusplus
}
#endif
#endif
