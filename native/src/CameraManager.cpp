#ifdef USE_LIBCAM

#include <jni.h>
#include <libcamera/libcamera.h>
#include "CameraHolder.hpp"

using namespace libcamera;

/*
 * Class:     frc1318_libcamera_CameraManager
 * Method:    createCameraManager
 * Signature: ()J
 */
extern "C"
JNIEXPORT jlong JNICALL Java_frc1318_libcamera_CameraManager_createCameraManager
    (JNIEnv* env, jclass objClass)
{
    CameraManager* cm = new CameraManager();
    cm->start();

    return reinterpret_cast<jlong>(cm);
}

/*
 * Class:     frc1318_libcamera_CameraManager
 * Method:    getCameraIds
 * Signature: (J)[Ljava/lang/String;
 */
extern "C"
JNIEXPORT jobjectArray JNICALL Java_frc1318_libcamera_CameraManager_getCameraIds
    (JNIEnv* env, jclass objClass, jlong nativeObj)
{
    CameraManager* cm = reinterpret_cast<CameraManager*>(nativeObj);

    std::vector<std::shared_ptr<Camera>> cameras = cm->cameras();

    jclass stringClass = env->FindClass("java/lang/String");
    jobjectArray cameraIdResults = env->NewObjectArray((jsize)cameras.size(), stringClass, NULL);
    for (int i = 0; i < cameras.size(); i++)
    {
        std::shared_ptr<Camera> camera = cameras[i];
        std::string camId = camera->id();
        jstring jcamId = env->NewStringUTF(camId.c_str());
        env->SetObjectArrayElement(cameraIdResults, i, jcamId);
    }

    return cameraIdResults;
}

/*
 * Class:     frc1318_libcamera_CameraManager
 * Method:    getCamera
 * Signature: (JLjava/lang/String;)J
 */
extern "C"
JNIEXPORT jlong JNICALL Java_frc1318_libcamera_CameraManager_getCamera
    (JNIEnv* env, jclass objClass, jlong nativeObj, jstring jcamId)
{
    const char *camIdUtfChars = env->GetStringUTFChars(jcamId, 0);
    std::string camId(camIdUtfChars);
    env->ReleaseStringUTFChars(jcamId, camIdUtfChars);

    CameraManager* cm = reinterpret_cast<CameraManager*>(nativeObj);

    std::shared_ptr<libcamera::Camera> camera = cm->get(camId);
    if (camera == nullptr)
    {
        // couldn't find camera with provided id?
        return static_cast<jlong>(0);
    }

    if (0 != camera->acquire())
    {
        // unable to acquire camera due to some error (already opened?)
        return static_cast<jlong>(0);
    }

    CameraHolder* cameraHolder = new CameraHolder(camera);
    return reinterpret_cast<jlong>(cameraHolder);
}

/*
 * Class:     frc1318_libcamera_CameraManager
 * Method:    release
 * Signature: (J)V
 */
extern "C"
JNIEXPORT void JNICALL Java_frc1318_libcamera_CameraManager_release
    (JNIEnv* env, jclass objClass, jlong nativeObj)
{
    CameraManager* cm = reinterpret_cast<CameraManager*>(nativeObj);
    cm->stop();
    delete cm;
}

#endif