/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class frc1318_libcamera_Camera */

#ifndef _Included_frc1318_libcamera_Camera
#define _Included_frc1318_libcamera_Camera
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     frc1318_libcamera_Camera
 * Method:    configure
 * Signature: (JIIII)I
 */
JNIEXPORT jint JNICALL Java_frc1318_libcamera_Camera_configure
  (JNIEnv *, jclass, jlong, jint, jint, jint, jint);

/*
 * Class:     frc1318_libcamera_Camera
 * Method:    updateSettings
 * Signature: (JFF)V
 */
JNIEXPORT void JNICALL Java_frc1318_libcamera_Camera_updateSettings
  (JNIEnv *, jclass, jlong, jfloat, jfloat);

/*
 * Class:     frc1318_libcamera_Camera
 * Method:    startCapturing
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_frc1318_libcamera_Camera_startCapturing
  (JNIEnv *, jclass, jlong);

/*
 * Class:     frc1318_libcamera_Camera
 * Method:    stopCapturing
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_frc1318_libcamera_Camera_stopCapturing
  (JNIEnv *, jclass, jlong);

/*
 * Class:     frc1318_libcamera_Camera
 * Method:    getControls
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_frc1318_libcamera_Camera_getControls
  (JNIEnv *, jclass, jlong);

/*
 * Class:     frc1318_libcamera_Camera
 * Method:    read
 * Signature: (J)Lorg/opencv/core/Mat;
 */
JNIEXPORT jobject JNICALL Java_frc1318_libcamera_Camera_read
  (JNIEnv *, jclass, jlong);

/*
 * Class:     frc1318_libcamera_Camera
 * Method:    delete
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_frc1318_libcamera_Camera_delete
  (JNIEnv *, jclass, jlong);

#ifdef __cplusplus
}
#endif
#endif
