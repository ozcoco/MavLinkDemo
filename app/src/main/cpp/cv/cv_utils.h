//
// Created by Administrator on 2020/1/7.
//

#ifndef MAVLINK_CV_UTILS_H
#define MAVLINK_CV_UTILS_H

#include <opencv2/opencv.hpp>
#include <jni.h>

using namespace cv;

namespace mcv {

    inline namespace utils {
        void nv21_2_gray(char *src, char *dst, int height, int width) noexcept;

        void gray(Mat &dst, Mat &src) noexcept;
    }

    inline namespace cvt {

        void
        bitmap_2_mat(JNIEnv *env, jobject bmp, Mat &mat,
                     const bool &needUnPremultiplyAlpha = false) noexcept;

        void mat_2_bitmap(JNIEnv *env, Mat &mat, jobject &bmp,
                          const bool &needPremultiplyAlpha = false) noexcept;

        Mat nv21_2_mat_RGB(uint8_t *nv21, int width, int height) noexcept;

        void nv21_2_mat_RGB(Mat &dst, uint8_t *nv21, int width, int height) noexcept;

        void nv21_2_mat_RGBA(Mat &dst, uint8_t *nv21, int width, int height) noexcept;
    }

}

#endif //MAVLINK_CV_UTILS_H


