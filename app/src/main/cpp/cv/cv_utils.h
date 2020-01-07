//
// Created by Administrator on 2020/1/7.
//

#ifndef MAVLINK_CV_UTILS_H
#define MAVLINK_CV_UTILS_H

#include <opencv2/opencv.hpp>

using namespace cv;

namespace mcv {

    inline namespace utils {
        void nv21_2_gray(char *src, char *dst, int height, int width) noexcept;
    }

}

#endif //MAVLINK_CV_UTILS_H


