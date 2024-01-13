#include <jni.h>
#include <apriltag.h>
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

extern "C" /* specify the C calling convention */
{
    using namespace cv;

    /*
     * Class:     frc1318_apriltag_AprilTagDetector
     * Method:    detect
     * Signature: (JJ)[Ljava/lang/Object;
     */
    JNIEXPORT jobjectArray JNICALL Java_frc1318_apriltag_AprilTagDetector_detect
        (JNIEnv* env, jclass objClass, jlong nativeObj, jlong imageObj)
    {
        Mat* mat = (Mat*)imageObj;

        // Make an image_u8_t from the Mat image
        image_u8_t image =
            {
                .width = mat->cols,
                .height = mat->rows,
                .stride = mat->cols,
                .buf = mat->data
            };

        apriltag_detector* apriltagDetector = (apriltag_detector*)nativeObj;

        // detect april tags within the image
        zarray_t* detections = apriltag_detector_detect(apriltagDetector, &image);

        // gather information about classes/constructors we are using (TODO: consider caching these somewhere?)
        jclass detectionClass = env->FindClass("frc1318/apriltag/AprilTagDetection");
        jmethodID detectionConstructor = env->GetMethodID(detectionClass, "<init>", "(JIIFLorg/opencv/core/Point;)V");
        jclass pointClass = env->FindClass("org/opencv/core/Point");
        jmethodID pointConstructor = env->GetMethodID(pointClass, "<init>", "(DD)V");

        // cycle through detections, convert them into a java object
        int detectionCount = zarray_size(detections);
        jobjectArray detectionResults = env->NewObjectArray((jsize)detectionCount, detectionClass, NULL);
        for (int i = 0; i < detectionCount; i++)
        {
            // convert the detection into the corresponding java object
            apriltag_detection_t* det;
            zarray_get(detections, i, &det);

            jobject centerPoint = env->NewObject(pointClass, pointConstructor, det->c[0], det->c[1]);

            jobject detectionResult =
                env->NewObject(
                    detectionClass,
                    detectionConstructor,
                    (jlong)det,
                    det->id,
                    det->hamming,
                    det->decision_margin,
                    centerPoint);

            env->SetObjectArrayElement(detectionResults, i, detectionResult);
        }

        zarray_destroy(detections);

        return detectionResults;
    }

    /*
     * Class:     frc1318_apriltag_AprilTagDetector
     * Method:    destroy
     * Signature: (J)V
     */
    JNIEXPORT void JNICALL Java_frc1318_apriltag_AprilTagDetector_destroy
        (JNIEnv* env, jclass objClass, jlong nativeObj)
    {
        apriltag_detector* td = (apriltag_detector*)nativeObj;
        apriltag_detector_destroy(td);
    }
}