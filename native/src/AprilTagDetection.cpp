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
#include "Mat4.hpp"

extern "C" /* specify the C calling convention */
{
    using namespace cv;

    const double METERS_PER_INCH = 0.0254;
    const double INCHES_PER_METER = 39.3700787;

    /*
     * Class:     frc1318_apriltag_AprilTagDetection
     * Method:    getHomographyMatrix
     * Signature: (J)[D
     */
    JNIEXPORT jdoubleArray JNICALL Java_frc1318_apriltag_AprilTagDetection_getHomographyMatrix
        (JNIEnv* env, jclass objClass, jlong detObj)
    {
        apriltag_detection_t* det = (apriltag_detection_t*)detObj;

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
    JNIEXPORT jobjectArray JNICALL Java_frc1318_apriltag_AprilTagDetection_getVertices
        (JNIEnv* env, jclass objClass, jlong detObj)
    {
        apriltag_detection_t* det = (apriltag_detection_t*)detObj;

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
    JNIEXPORT jobject JNICALL Java_frc1318_apriltag_AprilTagDetection_estimateTagPose
        (JNIEnv* env, jclass objClass, jlong detObj, jdouble tagSize, jdouble fx, jdouble fy, jdouble cx, jdouble cy, jlong t_camera_rel_robotObj, jdoubleArray offsetResults, jdoubleArray yprResults)
    {
        apriltag_detection_info_t info;
        info.det = (apriltag_detection_t*)detObj;
        info.tagsize = (tagSize * METERS_PER_INCH);
        info.fx = fx;
        info.fy = fy;
        info.cx = cx;
        info.cy = cy;

        Matx44d* t_camera_rel_robot = (Matx44d*)t_camera_rel_robotObj;

        apriltag_pose_t pose;
        double err = estimate_tag_pose(&info, &pose);

        jclass mat4WrapperClass = env->FindClass("frc1318/apriltag/Mat4");
        jmethodID mat4WrapperConstructor = env->GetMethodID(mat4WrapperClass, "<init>", "(J)V");

        jclass aprilTagPoseClass = env->FindClass("frc1318/apriltag/AprilTagPose");
        jmethodID aprilTagPoseConstructor = env->GetMethodID(aprilTagPoseClass, "<init>", "(Lfrc1318/apriltag/Mat4;D)V");

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

        Matx44d* t_apriltag_rel_robot = new Matx44d((*t_camera_rel_robot) * (*t_apriltag_rel_camera));

        delete t_apriltag_rel_camera;

        jobject wrappedTransformationMatrix = env->NewObject(mat4WrapperClass, mat4WrapperConstructor, (jlong)t_apriltag_rel_robot);
        jobject aprilTagPose = env->NewObject(aprilTagPoseClass, aprilTagPoseConstructor, wrappedTransformationMatrix, err);

        double yprArray[3]{ 0.0 };
        extract_ypr(t_apriltag_rel_robot, yprArray[0], yprArray[1], yprArray[2], 0, 1, 1);
        yprArray[0] *= DEGREES_PER_RADIAN;
        yprArray[1] *= DEGREES_PER_RADIAN;
        yprArray[2] *= DEGREES_PER_RADIAN;
        env->SetDoubleArrayRegion(yprResults, 0, 3, yprArray);

        double offArray[3]{ 0.0 };
        offArray[0] = t_apriltag_rel_robot->val[0 * 4 + 3];
        offArray[1] = t_apriltag_rel_robot->val[1 * 4 + 3];
        offArray[2] = t_apriltag_rel_robot->val[2 * 4 + 3];
        env->SetDoubleArrayRegion(offsetResults, 0, 3, offArray);

        return aprilTagPose;
    }

    /*
     * Class:     frc1318_apriltag_AprilTagDetection
     * Method:    destroy
     * Signature: (J)V
     */
    JNIEXPORT void JNICALL Java_frc1318_apriltag_AprilTagDetection_destroy
        (JNIEnv* env, jclass objClass, jlong detObj)
    {
        apriltag_detection_t* det = (apriltag_detection_t*)detObj;
        apriltag_detection_destroy(det);
    }
}