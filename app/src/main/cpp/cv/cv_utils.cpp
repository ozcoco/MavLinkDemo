//
// Created by Administrator on 2020/1/7.
//

#include "cv_utils.h"
#include <android/bitmap.h>
#include <utils/log.h>

void mcv::cvt::nv21_2_mat_RGBA(Mat &dst, uint8_t *nv21, int width, int height) noexcept {

    Mat nv21Mat{height + height / 2, width, CV_8UC1, nv21};

    cvtColor(nv21Mat, dst, COLOR_YUV2RGBA_NV21, 4);
}

void mcv::cvt::nv21_2_mat_RGB(Mat &dst, uint8_t *nv21, int width, int height) noexcept {

    Mat nv21Mat{height + height / 2, width, CV_8UC1, nv21};

    cvtColor(nv21Mat, dst, COLOR_YUV2RGB_NV21, 3);
}

Mat mcv::cvt::nv21_2_mat_RGB(uint8_t *nv21, int width, int height) noexcept {

    Mat nv21Mat{height + height / 2, width, CV_8UC1, nv21};

    Mat mat;

    cvtColor(nv21Mat, mat, COLOR_YUV2RGB_NV21, 3);

    return mat;
}

void mcv::cvt::bitmap_2_mat(JNIEnv *env, jobject bmp, Mat &mat,
                            const bool &needUnPremultiplyAlpha) noexcept {

    AndroidBitmapInfo info;
    void *pixels = nullptr;
    Mat &dst = mat;

    try {
        LOGD("nBitmapToMat");
        CV_Assert(AndroidBitmap_getInfo(env, bmp, &info) >= 0);
        CV_Assert(info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ||
                  info.format == ANDROID_BITMAP_FORMAT_RGB_565);
        CV_Assert(AndroidBitmap_lockPixels(env, bmp, &pixels) >= 0);
        CV_Assert(pixels);
        dst.create(info.height, info.width, CV_8UC4);
        if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
            LOGD("nBitmapToMat: RGBA_8888 -> CV_8UC4");
            Mat tmp(info.height, info.width, CV_8UC4, pixels);
            if (needUnPremultiplyAlpha) cvtColor(tmp, dst, COLOR_mRGBA2RGBA);
            else tmp.copyTo(dst);
        } else {
            // info.format == ANDROID_BITMAP_FORMAT_RGB_565
            LOGD("nBitmapToMat: RGB_565 -> CV_8UC4");
            Mat tmp(info.height, info.width, CV_8UC2, pixels);
            cvtColor(tmp, dst, COLOR_BGR5652RGBA);
        }
        AndroidBitmap_unlockPixels(env, bmp);
        return;
    } catch (const cv::Exception &e) {
        AndroidBitmap_unlockPixels(env, bmp);
        LOGE("nBitmapToMat catched cv::Exception: %s", e.what());
        jclass je = env->FindClass("org/opencv/core/CvException");
        if (!je) je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, e.what());
        return;
    } catch (...) {
        AndroidBitmap_unlockPixels(env, bmp);
        LOGE("nBitmapToMat catched unknown exception (...)");
        jclass je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, "Unknown exception in JNI code {nBitmapToMat}");
        return;
    }

}

void mcv::cvt::mat_2_bitmap(JNIEnv *env, Mat &mat, jobject &bmp,
                            const bool &needPremultiplyAlpha) noexcept {

    AndroidBitmapInfo info;

    void *pixels = nullptr;

    Mat &src = mat;

    try {
        LOGD("nMatToBitmap");
        CV_Assert(AndroidBitmap_getInfo(env, bmp, &info) >= 0);
        CV_Assert(info.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ||
                  info.format == ANDROID_BITMAP_FORMAT_RGB_565);
        CV_Assert(src.dims == 2 && info.height == (uint32_t) src.rows &&
                  info.width == (uint32_t) src.cols);
        CV_Assert(src.type() == CV_8UC1 || src.type() == CV_8UC3 || src.type() == CV_8UC4);
        CV_Assert(AndroidBitmap_lockPixels(env, bmp, &pixels) >= 0);
        CV_Assert(pixels);
        if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
            Mat tmp(info.height, info.width, CV_8UC4, pixels);
            if (src.type() == CV_8UC1) {
                LOGD("nMatToBitmap: CV_8UC1 -> RGBA_8888");
                cvtColor(src, tmp, COLOR_GRAY2RGBA);
            } else if (src.type() == CV_8UC3) {
                LOGD("nMatToBitmap: CV_8UC3 -> RGBA_8888");
                cvtColor(src, tmp, COLOR_RGB2RGBA);
            } else if (src.type() == CV_8UC4) {
                LOGD("nMatToBitmap: CV_8UC4 -> RGBA_8888");
                if (needPremultiplyAlpha)
                    cvtColor(src, tmp, COLOR_RGBA2mRGBA);
                else
                    src.copyTo(tmp);
            }
        } else {
            // info.format == ANDROID_BITMAP_FORMAT_RGB_565
            Mat tmp(info.height, info.width, CV_8UC2, pixels);
            if (src.type() == CV_8UC1) {
                LOGD("nMatToBitmap: CV_8UC1 -> RGB_565");
                cvtColor(src, tmp, COLOR_GRAY2BGR565);
            } else if (src.type() == CV_8UC3) {
                LOGD("nMatToBitmap: CV_8UC3 -> RGB_565");
                cvtColor(src, tmp, COLOR_RGB2BGR565);
            } else if (src.type() == CV_8UC4) {
                LOGD("nMatToBitmap: CV_8UC4 -> RGB_565");
                cvtColor(src, tmp, COLOR_RGBA2BGR565);
            }
        }
        AndroidBitmap_unlockPixels(env, bmp);
        return;
    } catch (const cv::Exception &e) {
        AndroidBitmap_unlockPixels(env, bmp);
        LOGE("nMatToBitmap catched cv::Exception: %s", e.what());
        jclass je = env->FindClass("org/opencv/core/CvException");
        if (!je) je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, e.what());
        return;
    } catch (...) {
        AndroidBitmap_unlockPixels(env, bmp);
        LOGE("nMatToBitmap catched unknown exception (...)");
        jclass je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, "Unknown exception in JNI code {nMatToBitmap}");
        return;
    }

}


void mcv::utils::gray(Mat &dst, Mat &src) noexcept {

    cvtColor(src, dst, COLOR_RGBA2GRAY);

}


void mcv::utils::nv21_2_gray(char *src, char *dst, int height, int width) noexcept {

    Mat srcMat(height * 3 / 2, width, CV_8UC1, src);

    Mat dstMat;

    cvtColor(srcMat, dstMat, COLOR_YUV2BGR_NV21);

    srcMat.release();

    cvtColor(dstMat, srcMat, COLOR_BGR2GRAY);

    dstMat.release();

    cvtColor(srcMat, dstMat, COLOR_BGR2YUV_I420);

    srcMat.release();

    memcpy(dst, dstMat.data, static_cast<size_t>(height * 3 / 2 + width));

    dstMat.release();

}


