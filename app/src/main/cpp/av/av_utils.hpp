//
// Created by Administrator on 2020/1/11.
//

#ifndef MAVLINK_AV_UTILS_HPP
#define MAVLINK_AV_UTILS_HPP

#ifdef __cplusplus
extern "C"
{
#endif

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include "libavutil/imgutils.h"

#ifdef __cplusplus
};
#endif

namespace mav {

    inline namespace cvt {

        inline int nv21_2_avframe(AVFrame *frame, uint8_t *yuv, int width, int height) noexcept {

            if (!frame)
                return -1;

            return av_image_fill_arrays(frame->data, frame->linesize, yuv,
                                        AV_PIX_FMT_NV21, width, height, 1);
        }

        inline int nv12_2_avframe(AVFrame *frame, uint8_t *yuv, int width, int height) noexcept {

            if (!frame)
                return -1;

            return av_image_fill_arrays(frame->data, frame->linesize, yuv,
                                        AV_PIX_FMT_NV12, width, height, 1);
        }

        inline int yuv420p_2_avframe(AVFrame *frame, uint8_t *yuv, int width, int height) noexcept {

            if (!frame)
                return -1;

            return av_image_fill_arrays(frame->data, frame->linesize, yuv,
                                        AV_PIX_FMT_YUV420P, width, height, 1);
        }

    }


}

#endif //MAVLINK_AV_UTILS_HPP
