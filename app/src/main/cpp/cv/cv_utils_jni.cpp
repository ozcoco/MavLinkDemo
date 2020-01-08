//
// Created by Administrator on 2020/1/7.
//

#include "cv_utils.h"

#include <jni.h>
#include <opencv2/opencv.hpp>
#include <memory>
#include <android/bitmap.h>
#include "utils/log.h"
#include <array>


using namespace mcv;
using namespace cv;


extern "C"
JNIEXPORT void
JNICALL
Java_me_oz_demo_mavlink_utils_CvUtils_nativeYUV420888Gray(JNIEnv *env, jclass type, jobject yBuff,
                                                          jobject uBuff, jobject vBuff, jint width,
                                                          jint height) {

    const jlong &ySize = env->GetDirectBufferCapacity(yBuff);

    const jlong &uSize = env->GetDirectBufferCapacity(uBuff);

    const jlong &vSize = env->GetDirectBufferCapacity(vBuff);

    char *y = static_cast<char *>(env->GetDirectBufferAddress(yBuff));

    char *u = static_cast<char *>(env->GetDirectBufferAddress(uBuff));

    char *v = static_cast<char *>(env->GetDirectBufferAddress(vBuff));

    const auto yuvSize = static_cast<const size_t>(height + height / 2, width);

    std::unique_ptr<char> yuv{new char[yuvSize]};

    std::copy(y, y + ySize, yuv.get());

    std::copy(u, u + uSize, yuv.get() + ySize);

    std::copy(v, v + vSize, yuv.get() + ySize + uSize);

    Mat src{height + height / 2, width, CV_8UC3, yuv.get()};

    Mat dst;

    cvtColor(src, dst, COLOR_YUV2GRAY_420, 1);

    memcpy(y, dst.data, (size_t) height * width);

    memcpy(u, dst.data + height * width, (size_t) height * width / 4);

    memcpy(v, dst.data + height * width + height * width / 4, (size_t) height * width / 4);

}

extern "C"
JNIEXPORT void
JNICALL
Java_me_oz_demo_mavlink_utils_CvUtils_nativeBitmapGray(JNIEnv *env, jclass type, jobject bmp) {

    Mat src;

    bitmap_2_mat(env, bmp, src);

    Mat dst;

    gray(dst, src);

    blur(src, dst, Size(20, 20));

    mat_2_bitmap(env, dst, bmp);
}


extern "C"
JNIEXPORT jbyteArray
JNICALL Java_me_oz_demo_mavlink_utils_CvUtils_nativeNV21Gray(JNIEnv *env, jclass type,
                                                             jbyteArray yuv, jint width,
                                                             jint height) {

//    jbyte *dBuf = env->GetDirectBufferAddress(env, buf);

    const jsize len = env->GetArrayLength(yuv);

    jboolean isCopy;

    char *buf = reinterpret_cast<char *>(env->GetByteArrayElements(yuv, &isCopy));

//    char *dst = nullptr;

//    mcv::nv21_2_gray(buf.get(), dst, height, width);

    jbyteArray dstData = env->NewByteArray(len);

    env->SetByteArrayRegion(dstData, 0, len, reinterpret_cast<const jbyte *>(buf));

//    env->ReleaseByteArrayElements(yuv, reinterpret_cast<jbyte *>(buf.get()), 0);

    return dstData;
}
