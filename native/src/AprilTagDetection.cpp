#include <jni.h>
#include <apriltag.h>
#include <apriltag_pose.h>
#include <tag36h10.h>
#include <tag36h11.h>
#include <tag25h9.h>
#include <tag16h5.h>
#include <tagCircle21h7.h>
#include <tagCircle49h12.h>
#include <tagCustom48h12.h>
#include <tagStandard41h12.h>
#include <tagStandard52h13.h>
#include <opencv2/opencv.hpp>
#include "Mat4.h"

using namespace cv;

const double METERS_PER_INCH = 0.0254;
const double INCHES_PER_METER = 39.3700787;

/*
 * Class:     frc1318_apriltag_AprilTagDetection
 * Method:    getHomographyMatrix
 * Signature: (J)[D
 */
extern "C"
JNIEXPORT jdoubleArray JNICALL Java_frc1318_apriltag_AprilTagDetection_getHomographyMatrix
    (JNIEnv* env, jclass objClass, jlong nativeObj)
{
    apriltag_detection_t* det = reinterpret_cast<apriltag_detection_t*>(nativeObj);

    int hMatrixSize = (det->H->nrows) * (det->H->ncols);
    jdoubleArray hMatrix = env->NewDoubleArray(hMatrixSize);
    env->SetDoubleArrayRegion(hMatrix, 0, hMatrixSize, det->H->data);

    return hMatrix;
}

/*
 * Class:     frc1318_apriltag_AprilTagDetection
 * Method:    getVertices
 * Signature: (J)[Lorg/opencv/core/Point;
 */
extern "C"
JNIEXPORT jobjectArray JNICALL Java_frc1318_apriltag_AprilTagDetection_getVertices
    (JNIEnv* env, jclass objClass, jlong nativeObj)
{
    apriltag_detection_t* det = reinterpret_cast<apriltag_detection_t*>(nativeObj);

    jclass pointClass = env->FindClass("org/opencv/core/Point");
    jmethodID pointConstructor = env->GetMethodID(pointClass, "<init>", "(DD)V");

    jobjectArray points = env->NewObjectArray(4, pointClass, NULL);
    for (int j = 0; j < 4; j++)
    {
        jobject point = env->NewObject(pointClass, pointConstructor, det->p[j][0], det->p[j][1]);
        env->SetObjectArrayElement(points, j, point);
    }

    return points;
}

/*
 * Class:     frc1318_apriltag_AprilTagDetection
 * Method:    estimateTagPose
 * Signature: (JDDDDDJ[D[D)Ljava/lang/Object;
 */
extern "C"
JNIEXPORT jobject JNICALL Java_frc1318_apriltag_AprilTagDetection_estimateTagPose
    (JNIEnv* env, jclass objClass, jlong nativeObj, jdouble tagSize, jdouble fx, jdouble fy, jdouble cx, jdouble cy, jlong t_camera_rel_bodyObj, jdoubleArray offsetResults, jdoubleArray yprResults)
{
    apriltag_detection_info_t info;
    info.det = reinterpret_cast<apriltag_detection_t*>(nativeObj);
    info.tagsize = (tagSize * METERS_PER_INCH);
    info.fx = fx;
    info.fy = fy;
    info.cx = cx;
    info.cy = cy;

    Matx44d* t_camera_rel_body = reinterpret_cast<Matx44d*>(t_camera_rel_bodyObj);

    apriltag_pose_t pose;
    double err = estimate_tag_pose(&info, &pose);

    jclass mat4WrapperClass = env->FindClass("frc1318/opencv/Mat4");
    jmethodID mat4WrapperConstructor = env->GetMethodID(mat4WrapperClass, "<init>", "(J)V");

    jclass aprilTagPoseClass = env->FindClass("frc1318/apriltag/AprilTagPose");
    jmethodID aprilTagPoseConstructor = env->GetMethodID(aprilTagPoseClass, "<init>", "(Lfrc1318/opencv/Mat4;D)V");

    // create a 3x3 rotation matrix to extract yaw, pitch, and roll from:
    Matx33d* r_apriltag_rel_camera_orig = new Matx33d();
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            r_apriltag_rel_camera_orig->operator()(i, j) = pose.R->data[3 * i + j];
        }
    }

    double originalYaw, originalPitch, originalRoll;
    extract_ypr(r_apriltag_rel_camera_orig, originalYaw, originalPitch, originalRoll, 0, 1, 1);

    delete r_apriltag_rel_camera_orig;

    Matx44d* t_apriltag_rel_camera =
        createAff(
            -originalPitch,
            -originalRoll,
            originalYaw,
            (pose.t->data[2] * INCHES_PER_METER),
            -(pose.t->data[0] * INCHES_PER_METER),
            -(pose.t->data[1] * INCHES_PER_METER),
            1,
            false);

    // destroy the things that came back from the apriltag library
    matd_destroy(pose.R);
    matd_destroy(pose.t);

    Matx44d* t_apriltag_rel_body = new Matx44d((*t_camera_rel_body) * (*t_apriltag_rel_camera));

    delete t_apriltag_rel_camera;

    jobject wrappedTransformationMatrix = env->NewObject(mat4WrapperClass, mat4WrapperConstructor, (jlong)t_apriltag_rel_body);
    jobject aprilTagPose = env->NewObject(aprilTagPoseClass, aprilTagPoseConstructor, wrappedTransformationMatrix, err);

    double yprArray[3]{ 0.0 };
    extract_ypr(t_apriltag_rel_body, yprArray[0], yprArray[1], yprArray[2], 0, 1, 1);
    yprArray[0] *= DEGREES_PER_RADIAN;
    yprArray[1] *= DEGREES_PER_RADIAN;
    yprArray[2] *= DEGREES_PER_RADIAN;
    env->SetDoubleArrayRegion(yprResults, 0, 3, yprArray);

    double offArray[3]{ 0.0 };
    offArray[0] = t_apriltag_rel_body->val[0 * 4 + 3];
    offArray[1] = t_apriltag_rel_body->val[1 * 4 + 3];
    offArray[2] = t_apriltag_rel_body->val[2 * 4 + 3];
    env->SetDoubleArrayRegion(offsetResults, 0, 3, offArray);

    return aprilTagPose;
}

/*
* Class:     frc1318_apriltag_AprilTagDetection
* Method:    estimateAbsolutePose
* Signature: (JDDDDDJJ[D[D)Ljava/lang/Object;
*/
extern "C"
JNIEXPORT jobject JNICALL Java_frc1318_apriltag_AprilTagDetection_estimateAbsolutePose
    (JNIEnv* env, jclass objClass, jlong nativeObj, jdouble tagSize, jdouble fx, jdouble fy, jdouble cx, jdouble cy, jlong t_body_rel_cameraObj, jlong t_apriltag_rel_absoluteObj, jdoubleArray offsetResults, jdoubleArray yprResults)
{
    apriltag_detection_info_t info;
    info.det = reinterpret_cast<apriltag_detection_t*>(nativeObj);
    info.tagsize = (tagSize * METERS_PER_INCH);
    info.fx = fx;
    info.fy = fy;
    info.cx = cx;
    info.cy = cy;

    Matx44d* t_body_rel_camera = reinterpret_cast<Matx44d*>(t_body_rel_cameraObj);
    Matx44d* t_apriltag_rel_absolute = reinterpret_cast<Matx44d*>(t_apriltag_rel_absoluteObj);

    apriltag_pose_t pose;
    double err = estimate_tag_pose(&info, &pose);

    jclass mat4WrapperClass = env->FindClass("frc1318/opencv/Mat4");
    jmethodID mat4WrapperConstructor = env->GetMethodID(mat4WrapperClass, "<init>", "(J)V");

    jclass aprilTagPoseClass = env->FindClass("frc1318/apriltag/AprilTagPose");
    jmethodID aprilTagPoseConstructor = env->GetMethodID(aprilTagPoseClass, "<init>", "(Lfrc1318/opencv/Mat4;D)V");

    // create a 3x3 rotation matrix to extract yaw, pitch, and roll from:
    Matx33d* r_apriltag_rel_camera_orig = new Matx33d();
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            r_apriltag_rel_camera_orig->operator()(i, j) = pose.R->data[3 * i + j];
        }
    }

    double originalYaw, originalPitch, originalRoll;
    extract_ypr(r_apriltag_rel_camera_orig, originalYaw, originalPitch, originalRoll, 0, 1, 1);

    delete r_apriltag_rel_camera_orig;

    Matx44d* t_apriltag_rel_camera =
        createAff(
            -originalPitch,
            -originalRoll,
            originalYaw,
            (pose.t->data[2] * INCHES_PER_METER),
            -(pose.t->data[0] * INCHES_PER_METER),
            -(pose.t->data[1] * INCHES_PER_METER),
            1,
            false);

    // destroy the things that came back from the apriltag library
    matd_destroy(pose.R);
    matd_destroy(pose.t);

    // invert the affine transformation matrix of the apriltag relative to the camera, delete the old apriltag
    Matx44d* t_camera_rel_apriltag = invAff(t_apriltag_rel_camera);
    delete t_apriltag_rel_camera;

    Matx44d* t_body_rel_absolute = new Matx44d((*t_apriltag_rel_absolute) * (*t_camera_rel_apriltag) * (*t_body_rel_camera));

    delete t_camera_rel_apriltag;

    jobject wrappedTransformationMatrix = env->NewObject(mat4WrapperClass, mat4WrapperConstructor, (jlong)t_body_rel_absolute);
    jobject aprilTagPose = env->NewObject(aprilTagPoseClass, aprilTagPoseConstructor, wrappedTransformationMatrix, err);

    double yprArray[3]{ 0.0 };
    extract_ypr(t_body_rel_absolute, yprArray[0], yprArray[1], yprArray[2], 0, 1, 1);
    yprArray[0] *= DEGREES_PER_RADIAN;
    yprArray[1] *= DEGREES_PER_RADIAN;
    yprArray[2] *= DEGREES_PER_RADIAN;
    env->SetDoubleArrayRegion(yprResults, 0, 3, yprArray);

    double offArray[3]{ 0.0 };
    offArray[0] = t_body_rel_absolute->val[0 * 4 + 3];
    offArray[1] = t_body_rel_absolute->val[1 * 4 + 3];
    offArray[2] = t_body_rel_absolute->val[2 * 4 + 3];
    env->SetDoubleArrayRegion(offsetResults, 0, 3, offArray);

    return aprilTagPose;
}

/*
 * Class:     frc1318_apriltag_AprilTagDetection
 * Method:    release
 * Signature: (J)V
 */
extern "C"
JNIEXPORT void JNICALL Java_frc1318_apriltag_AprilTagDetection_release
    (JNIEnv* env, jclass objClass, jlong nativeObj)
{
    apriltag_detection_t* det = reinterpret_cast<apriltag_detection_t*>(nativeObj);
    apriltag_detection_destroy(det);
}