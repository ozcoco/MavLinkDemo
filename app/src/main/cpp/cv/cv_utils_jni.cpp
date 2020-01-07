//
// Created by Administrator on 2020/1/7.
//

#include "cv_utils.h"

#include <jni.h>
#include <opencv2/opencv.hpp>


using namespace mcv;
using namespace cv;

extern "C"
JNIEXPORT jbyteArray
JNICALL Java_me_oz_demo_mavlink_utils_CvUtils_native_1NV21_1gray(JNIEnv *env, jclass type,
                                                                 jbyteArray yuv, jint width,
                                                                 jint height) {

    jsize len = env->GetArrayLength(yuv);

    jbyte *buf = NULL;

    env->GetByteArrayRegion(yuv, 0, len, buf);

    char *yuvData = reinterpret_cast<char *>(buf);

    jbyte *dst = NULL;

    mcv::nv21_2_gray(yuvData, reinterpret_cast<char *>(dst), height, width);

    env->ReleaseByteArrayElements(yuv, buf, 0);

    int dstSize = height * 3 / 2 + width;

    jbyteArray dstData = env->NewByteArray(dstSize);

    env->SetByteArrayRegion(dstData, 0, dstSize, dst);

    return dstData;
}
