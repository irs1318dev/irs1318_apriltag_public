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

/*
 * Class:     frc1318_apriltag_AprilTag
 * Method:    createDetector
 * Signature: (IIIFFZDZ)J
 */
extern "C"
JNIEXPORT jlong JNICALL Java_frc1318_apriltag_AprilTag_createDetector
    (JNIEnv* env, jclass objClass, jint family, jint hamming, jint nThreads, jfloat quadDecimate, jfloat quadSigma, jboolean refineEdges, jdouble decodeSharpening, jboolean debug)
{
    apriltag_family_t* tf = NULL;
    switch ((int)family)
    {
    case 0:
        tf = tag36h11_create();
        break;

    case 1:
        tf = tag25h9_create();
        break;

    case 2:
        tf = tag36h10_create();
        break;

    case 3:
        tf = tag16h5_create();
        break;

    case 4:
        tf = tagCircle21h7_create();
        break;

    case 5:
        tf = tagCircle49h12_create();
        break;

    case 6:
        tf = tagCustom48h12_create();
        break;

    case 7:
        tf = tagStandard41h12_create();
        break;

    case 8:
        tf = tagStandard52h13_create();
        break;
    }

    apriltag_detector_t* td = apriltag_detector_create();
    apriltag_detector_add_family_bits(td, tf, hamming);

    td->nthreads = nThreads;
    td->quad_decimate = quadDecimate;
    td->quad_sigma = quadSigma;
    td->refine_edges = refineEdges;
    td->decode_sharpening = decodeSharpening;
    td->debug = debug;

    return (jlong)td;
}
