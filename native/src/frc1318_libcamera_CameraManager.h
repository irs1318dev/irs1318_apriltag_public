/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class frc1318_libcamera_CameraManager */

#ifndef _Included_frc1318_libcamera_CameraManager
#define _Included_frc1318_libcamera_CameraManager
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     frc1318_libcamera_CameraManager
 * Method:    createCameraManager
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_frc1318_libcamera_CameraManager_createCameraManager
  (JNIEnv *, jclass);

/*
 * Class:     frc1318_libcamera_CameraManager
 * Method:    getCameraIds
 * Signature: (J)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_frc1318_libcamera_CameraManager_getCameraIds
  (JNIEnv *, jclass, jlong);

/*
 * Class:     frc1318_libcamera_CameraManager
 * Method:    getCamera
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_frc1318_libcamera_CameraManager_getCamera
  (JNIEnv *, jclass, jlong, jstring);

/*
 * Class:     frc1318_libcamera_CameraManager
 * Method:    release
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_frc1318_libcamera_CameraManager_release
  (JNIEnv *, jclass, jlong);

#ifdef __cplusplus
}
#endif
#endif
