//
// Created by Administrator on 2020/1/7.
//

#include "cv_utils.h"


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


