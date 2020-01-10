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

constexpr uint8_t YUV_COLOR_GRAY = 128;

extern "C"
JNIEXPORT void
JNICALL
Java_me_oz_demo_mavlink_utils_CvUtils_nativeYUV420888Bitmap565(JNIEnv *env, jclass type,
                                                               jobject yBuff,
                                                               jobject uBuff,
                                                               jobject vBuff,
                                                               jobject bmp,
                                                               jint width,
                                                               jint height) {

    const jlong &ySize = env->GetDirectBufferCapacity(yBuff);

    const jlong &uSize = env->GetDirectBufferCapacity(uBuff);

    const jlong &vSize = env->GetDirectBufferCapacity(vBuff);

    void *y = env->GetDirectBufferAddress(yBuff);

    void *u = env->GetDirectBufferAddress(uBuff);

    void *v = env->GetDirectBufferAddress(vBuff);

    std::unique_ptr<uint8_t[]> nv21 = std::make_unique<uint8_t[]>(
            static_cast<size_t>(ySize + vSize + uSize));

    memcpy(nv21.get(), y, (size_t) ySize);

    memcpy(nv21.get() + ySize, v, (size_t) vSize);

    memcpy(nv21.get() + ySize + vSize, u, (size_t) uSize);

    Mat src;

    nv21_2_mat_RGB(src, nv21.get(), width, height);

    Mat grayMat;

    cvtColor(src, grayMat, COLOR_RGB2GRAY);

    mat_2_bitmap(env, grayMat, bmp);
}


extern "C"
JNIEXPORT void
JNICALL
Java_me_oz_demo_mavlink_utils_CvUtils_nativeYUV420888Gray(JNIEnv *env, jclass type, jobject yBuff,
                                                          jobject uBuff, jobject vBuff, jint width,
                                                          jint height) {

    const jlong &ySize = env->GetDirectBufferCapacity(yBuff);

    const jlong &uSize = env->GetDirectBufferCapacity(uBuff);

    const jlong &vSize = env->GetDirectBufferCapacity(vBuff);

    auto *y = static_cast<uint8_t *>(env->GetDirectBufferAddress(yBuff));

    auto *u = static_cast<uint8_t *>(env->GetDirectBufferAddress(uBuff));

    auto *v = static_cast<uint8_t *>(env->GetDirectBufferAddress(vBuff));

    std::for_each(u, u + uSize, [](uint8_t &e) -> void {
        e = saturate_cast<uint8_t>(YUV_COLOR_GRAY);
    });

    std::for_each(v, v + vSize, [](uint8_t &e) -> void {
        e = saturate_cast<uint8_t>(YUV_COLOR_GRAY);
    });

    std::unique_ptr<uint8_t[]> nv21{new uint8_t[ySize + vSize + uSize]};

    memcpy(nv21.get(), y, (size_t) ySize);

    memcpy(nv21.get() + ySize, v, (size_t) vSize);

    memcpy(nv21.get() + ySize + vSize, u, (size_t) uSize);

    Mat nv21Mat{height + height / 2, width, CV_8UC1, nv21.get()};

    Mat dst;

    cvtColor(nv21Mat, dst, COLOR_YUV2GRAY_NV21);

    memcpy(y, dst.data, (size_t) ySize);

    memcpy(v, dst.data + ySize, (size_t) vSize);

    memcpy(u, dst.data + ySize + vSize, (size_t) uSize);

}

extern "C"
JNIEXPORT void
JNICALL
Java_me_oz_demo_mavlink_utils_CvUtils_nativeBitmapGray(JNIEnv *env, jclass type, jobject bmp) {

    Mat src;

    bitmap_2_mat(env, bmp, src);

    Mat dst;

//    gray(dst, src);

//    blur(src, dst, Size(20, 20));

    Mat_<char> kernel(3, 3);

    kernel << 0, -1, 0, -1, 5, -1, 0, -1, 0;

    filter2D(src, dst, dst.depth(), kernel);

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

extern "C"
JNIEXPORT
void JNICALL Java_me_oz_demo_mavlink_utils_CvUtils_destroy(JNIEnv *env, jclass type) {

}