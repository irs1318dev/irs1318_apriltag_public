#ifdef USE_LIBCAM

#include <jni.h>
#include <opencv2/opencv.hpp>
#include <libcamera/libcamera.h>
#include "CameraHolder.hpp"

using namespace cv;
using namespace libcamera;

/*
 * Class:     frc1318_libcamera_Camera
 * Method:    configure
 * Signature: (JIIII)I
 */
extern "C"
JNIEXPORT jint JNICALL Java_frc1318_libcamera_Camera_configure
    (JNIEnv* env, jclass objClass, jlong nativeObj, jint resX, jint resY, jint frameRate, jint format)
{
    CameraHolder* cameraHolder = reinterpret_cast<CameraHolder*>(nativeObj);
    return cameraHolder->configure(resX, resY, frameRate, static_cast<ImageFormat>(format));
}

/*
 * Class:     frc1318_libcamera_Camera
 * Method:    updateSettings
 * Signature: (JFF)V
 */
extern "C"
JNIEXPORT void JNICALL Java_frc1318_libcamera_Camera_updateSettings
    (JNIEnv* env, jclass objClass, jlong nativeObj, jfloat brightness, jfloat exposure)
{
    CameraHolder* cameraHolder = reinterpret_cast<CameraHolder*>(nativeObj);
    cameraHolder->updateSettings(brightness, exposure);
}

/*
 * Class:     frc1318_libcamera_Camera
 * Method:    startCapturing
 * Signature: (J)I
 */
extern "C"
JNIEXPORT jint JNICALL Java_frc1318_libcamera_Camera_startCapturing
    (JNIEnv* env, jclass objClass, jlong nativeObj)
{
    CameraHolder* cameraHolder = reinterpret_cast<CameraHolder*>(nativeObj);
    return cameraHolder->startCapturing();
}

/*
 * Class:     frc1318_libcamera_Camera
 * Method:    stopCapturing
 * Signature: (J)V
 */
extern "C"
JNIEXPORT void JNICALL Java_frc1318_libcamera_Camera_stopCapturing
    (JNIEnv* env, jclass objClass, jlong nativeObj)
{
    CameraHolder* cameraHolder = reinterpret_cast<CameraHolder*>(nativeObj);
    cameraHolder->stopCapturing();
}

/*
 * Class:     frc1318_libcamera_Camera
 * Method:    getControls
 * Signature: (J)Ljava/lang/String;
 */
extern "C"
JNIEXPORT jstring JNICALL Java_frc1318_libcamera_Camera_getControls
    (JNIEnv* env, jclass objClass, jlong nativeObj)
{
    CameraHolder* cameraHolder = reinterpret_cast<CameraHolder*>(nativeObj);

    std::string controls = cameraHolder->getControls();
    return env->NewStringUTF(controls.c_str()); // Note: this may have issues if the string happens to contain a \0 character, but it should be good enough for now..
}

/*
 * Class:     frc1318_libcamera_Camera
 * Method:    read
 * Signature: (J)Lorg/opencv/core/Mat;
 */
extern "C"
JNIEXPORT jobject JNICALL Java_frc1318_libcamera_Camera_read
    (JNIEnv* env, jclass objClass, jlong nativeObj)
{
    jclass matClass = env->FindClass("org/opencv/core/Mat");
    jmethodID matConstructor = env->GetMethodID(matClass, "<init>", "(J)V");

    CameraHolder* cameraHolder = reinterpret_cast<CameraHolder*>(nativeObj);
    Mat* result = cameraHolder->read();
    if (result == nullptr)
    {
        return nullptr;
    }

    return env->NewObject(matClass, matConstructor, reinterpret_cast<jlong>(result));
}

/*
 * Class:     frc1318_libcamera_Camera
 * Method:    delete
 * Signature: (J)V
 */
extern "C"
JNIEXPORT void JNICALL Java_frc1318_libcamera_Camera_delete
    (JNIEnv* env, jclass objClass, jlong nativeObj)
{
    CameraHolder* cameraHolder = reinterpret_cast<CameraHolder*>(nativeObj);
    cameraHolder->release();
    delete cameraHolder;
}

#endif