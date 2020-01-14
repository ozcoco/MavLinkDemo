//
// Created by Administrator on 2020/1/10.
//

#ifndef MAVLINK_PUSH_STREAM_HPP
#define MAVLINK_PUSH_STREAM_HPP

#include<string>
#include <utils/log.h>
#include "av_utils.hpp"
#include <cstdio>
#include <ctime>

#ifdef __cplusplus
extern "C"
{
#endif
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#ifdef __cplusplus
};
#endif

namespace mav {

    enum PUSH_STREAM_TYPE {
        NV21_2_H264,
        NV21_2_H265
    };

    enum PUSH_MIME_TYPE {
        MP4,
        FLV
    };

    enum PUSH_PROTOCOL_TYPE {
        RTMP,
        RTSP,
//        TCP,
//        UDP,
    };


    inline void custom_log_callback(void *ptr, int level, const char *fmt, va_list vl) noexcept {

        switch (level) {
            case AV_LOG_INFO:
                LOGI(fmt, vl);
                break;
            case AV_LOG_ERROR:
                LOGE(fmt, vl);
                break;
            case AV_LOG_DEBUG:
                LOGD(fmt, vl);
                break;
            case AV_LOG_WARNING:
                LOGW(fmt, vl);
                break;
            default:
                LOGI(fmt, vl);
                break;
        }
    }


    class PushStreamer {

    public:
        PushStreamer(const std::string &url, const PUSH_STREAM_TYPE &streamType = NV21_2_H264,
                     const PUSH_MIME_TYPE &mimeType = MP4,
                     const PUSH_PROTOCOL_TYPE &protocolType = RTSP) :
                url(url),
                streamType(streamType),
                mimeType(mimeType),
                protocolType(protocolType) {

            init();
        }

        virtual ~PushStreamer() {

            destroy();

        }


        inline void init() noexcept {

            initStreamConfig();

        }

        inline void destroy() noexcept {

            avformat_close_input(&ofmt_ctx);

            /* close output */
            if (ofmt_ctx && !(ofmt_ctx->flags & AVFMT_NOFILE))
                avio_closep(&ofmt_ctx->pb);

            avformat_free_context(ofmt_ctx);

            if (avctx) {
                avcodec_close(avctx);

                avcodec_free_context(&avctx);
            }

        }


        inline void initStreamConfig() noexcept {

            av_log_set_callback(custom_log_callback);

            int ret;

            //分配output context
            ret = avformat_alloc_output_context2(&ofmt_ctx, NULL, "flv", url.c_str());

            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "---> avformat_alloc_output_context2");
                destroy();
                return;
            }

            if (!(ofmt_ctx->flags & AVFMT_NOFILE)) {
                ret = avio_open(&ofmt_ctx->pb, url.c_str(), AVIO_FLAG_WRITE);
                if (ret < 0) {
                    av_log(NULL, AV_LOG_ERROR, "Could not open output file '%s'", url.c_str());
                    destroy();
                    return;
                }
            }

            out_stream = avformat_new_stream(ofmt_ctx, ofmt_ctx->video_codec);

            if (!out_stream) {
                av_log(NULL, AV_LOG_ERROR, "Error occurred when new stream \n");
                destroy();
                return;
            }

            avctx = avcodec_alloc_context3(ofmt_ctx->video_codec);

            if (!avctx) {
                av_log(NULL, AV_LOG_ERROR, "Error occurred when alloc context\n");
                destroy();
                return;
            }

            ret = avcodec_open2(avctx, ofmt_ctx->video_codec, NULL);

            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "Error occurred when codec open\n");
                destroy();
                return;
            }

            // 写入新的多媒体文件的头
            ret = avformat_write_header(ofmt_ctx, NULL);
            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "Error occurred when opening output file\n");
                destroy();
                return;
            }

            avpkt = av_packet_alloc();

            if (!avpkt) {
                av_log(NULL, AV_LOG_ERROR, "Error occurred when packet alloc\n");
                destroy();
                return;
            }

        };

        inline void
        push(unsigned char *nv21, const unsigned long int &size, int width,
             int height) noexcept {

            switch (streamType) {

                case NV21_2_H264:

                    if (protocolType == RTMP) {

                        pushByRTMP(nv21, size, width, height);

                    } else if (protocolType == RTSP) {

                        pushByRTSP(nv21, size, width, height);
                    }

                    break;

                case NV21_2_H265:

                    if (protocolType == RTMP) {

                        pushByRTMP(nv21, size, width, height);

                    } else if (protocolType == RTSP) {

                        pushByRTSP(nv21, size, width, height);
                    }

                    break;
            }

        }


        inline void pushByRTSP(unsigned char *nv21, const unsigned long int &size, int width,
                               int height) noexcept {}


        /***
         *
            1.av_register_all() : 注册组件, 包括FFmpeg所有编解码器
            2.avformat_alloc_context(): 初始化封装格式上下文
            3.avio_open(): 打开输入文件,关联封装格式上下文到输出文件
            4.avformat_new_stream(): 创建输出码流
            5.avcodec_find_encoder():查找视频编码器
            6.avcodec_open2(): 打开解码器
            7.avformat_write_header(): 写入文件头信息
            8.av_image_fill_arrays():存储一帧像素数据缓冲区
            9.读取帧av_read_frame() 返回值<0 则结束
            10.av_frame->data: 读取缓冲数据转成AVFrame类
            11.avcodec_send_frame(): 发送一帧视频像素数据(yuv格式)
            12.avcodec_receive_packet(): 接收一帧视频数据,编码为视频压缩格式(如H.264格式)
            13.av_write_frame():编码码成功写入文件,再循环读取下一帧
            14.flush_encoder():写入剩余的帧数据
            15.av_write_trailer():写入文件尾部信息
         * **/

        inline void pushByRTMP(unsigned char *nv21, const unsigned long int &size, int width,
                               int height) noexcept {

            int ret;

            AVFrame *yuvFrame = av_frame_alloc();

            yuvFrame->pts = time(NULL);

            ret = nv21_2_avframe(yuvFrame, nv21, width, height);

            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "Error occurred when nv21 to frame \n");
                destroy();
                return;
            }

            ret = avcodec_send_frame(avctx, yuvFrame);

            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "Error occurred when send frame \n");
                destroy();
                return;
            }

            av_packet_unref(avpkt);

            ret = avcodec_receive_packet(avctx, avpkt);

            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "Error occurred when receive packet \n");
                destroy();
                return;
            }

            AVRational stime = (AVRational) {1, 25};

            AVRational dtime = (AVRational) {1, 25};

            if (avpkt->stream_index == out_stream->index) {
                stime = avctx->time_base;
                dtime = out_stream->time_base;
            }

            avpkt->pts = av_rescale_q(avpkt->pts, stime, dtime);

            avpkt->dts = av_rescale_q(avpkt->dts, stime, dtime);

            avpkt->duration = av_rescale_q(avpkt->duration, stime, dtime);

            ret = av_interleaved_write_frame(ofmt_ctx, avpkt);

            if (ret < 0) {
                av_log(NULL, AV_LOG_ERROR, "Error occurred when nterleaved write frame \n");
                destroy();
                return;
            }

        }


//        inline void pushByTCP(unsigned char *nv21, const unsigned long int &size, int width,
//                              int height) noexcept {
//
//
//        }
//
//        inline void pushByUDP(unsigned char *nv21, const unsigned long int &size, int width,
//                              int height) noexcept {
//
//
//        }


        inline PUSH_STREAM_TYPE getStreamType() const {
            return streamType;
        }

        inline void setStreamType(PUSH_STREAM_TYPE streamType) {
            PushStreamer::streamType = streamType;
        }

        inline PUSH_MIME_TYPE getMimeType() const {
            return mimeType;
        }

        inline void setMimeType(PUSH_MIME_TYPE mimeType) {
            PushStreamer::mimeType = mimeType;
        }

        inline PUSH_PROTOCOL_TYPE getProtocolType() const {
            return protocolType;
        }

        inline void setProtocolType(PUSH_PROTOCOL_TYPE protocolType) {
            PushStreamer::protocolType = protocolType;
        }

    private:
        PUSH_STREAM_TYPE streamType;

        PUSH_MIME_TYPE mimeType;

        PUSH_PROTOCOL_TYPE protocolType;

        std::string url;

        AVFormatContext *ofmt_ctx;

        AVCodecContext *avctx;

        AVPacket *avpkt;

        AVStream *out_stream;
    };

}


#endif //MAVLINK_PUSH_STREAM_HPP
