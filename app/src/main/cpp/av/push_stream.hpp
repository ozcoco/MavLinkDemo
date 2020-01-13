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
                               int height) noexcept {

            const char in_filename[] = "E:/testVideo/20.mp4";

            const char out_filename[] = "E:/testVideo/20.flv";

            AVOutputFormat *ofmt = NULL;  // 输出格式

            AVFormatContext *ifmt_ctx = NULL, *ofmt_ctx = NULL; // 输入、输出是上下文环境

            AVPacket pkt;

            int ret, i;

            int stream_index = 0;

            int *stream_mapping = NULL; // 数组用于存放输出文件流的Index

            int stream_mapping_size = 0; // 输入文件中流的总数量

            if (ret < 0) {

                av_log(NULL, AV_LOG_ERROR, "Could not open input file '%s'", in_filename);

                goto end;
            }

            // 打开输入文件为ifmt_ctx分配内存
            if ((ret = avformat_open_input(&ifmt_ctx, in_filename, 0, 0)) < 0) {
                fprintf(stderr, "Could not open input file '%s'", in_filename);
                goto end;
            }

            // 检索输入文件的流信息
            if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0) {
                fprintf(stderr, "Failed to retrieve input stream information");
                goto end;
            }

            // 打印输入文件相关信息
            av_dump_format(ifmt_ctx, 0, in_filename, 0);

            // 为输出上下文环境分配内存
            avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, out_filename);
            if (!ofmt_ctx) {
                fprintf(stderr, "Could not create output context\n");
                ret = AVERROR_UNKNOWN;
                goto end;
            }

            // 输入文件流的数量
            stream_mapping_size = ifmt_ctx->nb_streams;

            // 分配stream_mapping_size段内存，每段内存大小是sizeof(*stream_mapping)
            stream_mapping = (int *) av_mallocz_array(stream_mapping_size, sizeof(*stream_mapping));
            if (!stream_mapping) {
                ret = AVERROR(ENOMEM);
                goto end;
            }

            // 输出文件格式
            ofmt = ofmt_ctx->oformat;

            // 遍历输入文件中的每一路流，对于每一路流都要创建一个新的流进行输出
            for (i = 0; i < ifmt_ctx->nb_streams; i++) {
                AVStream *out_stream; // 输出流
                AVStream *in_stream = ifmt_ctx->streams[i]; // 输入流
                AVCodecParameters *in_codecpar = in_stream->codecpar; // 输入流的编解码参数

                // 只保留音频、视频、字幕流，其他的流不需要
                if (in_codecpar->codec_type != AVMEDIA_TYPE_AUDIO &&
                    in_codecpar->codec_type != AVMEDIA_TYPE_VIDEO &&
                    in_codecpar->codec_type != AVMEDIA_TYPE_SUBTITLE) {
                    stream_mapping[i] = -1;
                    continue;
                }

                // 对于输出的流的index重写编号
                stream_mapping[i] = stream_index++;

                // 创建一个对应的输出流
                out_stream = avformat_new_stream(ofmt_ctx, NULL);
                if (!out_stream) {
                    fprintf(stderr, "Failed allocating output stream\n");
                    ret = AVERROR_UNKNOWN;
                    goto end;
                }

                // 直接将输入流的编解码参数拷贝到输出流中
                ret = avcodec_parameters_copy(out_stream->codecpar, in_codecpar);
                if (ret < 0) {
                    fprintf(stderr, "Failed to copy codec parameters\n");
                    goto end;
                }
                out_stream->codecpar->codec_tag = 0;
            }

            // 打印要输出的多媒体文件的详细信息
            av_dump_format(ofmt_ctx, 0, out_filename, 1);

            if (!(ofmt->flags & AVFMT_NOFILE)) {
                ret = avio_open(&ofmt_ctx->pb, out_filename, AVIO_FLAG_WRITE);
                if (ret < 0) {
                    fprintf(stderr, "Could not open output file '%s'", out_filename);
                    goto end;
                }
            }

            // 写入新的多媒体文件的头
            ret = avformat_write_header(ofmt_ctx, NULL);
            if (ret < 0) {
                fprintf(stderr, "Error occurred when opening output file\n");
                goto end;
            }

            while (1) {
                AVStream *in_stream, *out_stream;

                // 循环读取每一帧数据
                ret = av_read_frame(ifmt_ctx, &pkt);


                if (ret < 0) // 读取完后退出循环
                    break;

                in_stream = ifmt_ctx->streams[pkt.stream_index];
                if (pkt.stream_index >= stream_mapping_size ||
                    stream_mapping[pkt.stream_index] < 0) {
                    av_packet_unref(&pkt);
                    continue;
                }

                pkt.stream_index = stream_mapping[pkt.stream_index]; // 按照输出流的index给pkt重新编号
                out_stream = ofmt_ctx->streams[pkt.stream_index]; // 根据pkt的stream_index获取对应的输出流

                // 对pts、dts、duration进行时间基转换，不同格式时间基都不一样，不转换会导致音视频同步问题
                pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base,
                                           (AVRounding) (AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
                pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base,
                                           (AVRounding) (AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
                pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base,
                                            out_stream->time_base);
                pkt.pos = -1;

                // 将处理好的pkt写入输出文件
                ret = av_interleaved_write_frame(ofmt_ctx, &pkt);
                if (ret < 0) {
                    fprintf(stderr, "Error muxing packet\n");
                    break;
                }
                av_packet_unref(&pkt);
            }

            // 写入新的多媒体文件尾
            av_write_trailer(ofmt_ctx);
            end:

            avformat_close_input(&ifmt_ctx);

            /* close output */
            if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
                avio_closep(&ofmt_ctx->pb);
            avformat_free_context(ofmt_ctx);

            av_freep(&stream_mapping);

            if (ret < 0 && ret != AVERROR_EOF) {
                //fprintf(stderr, "Error occurred: %s\n", av_err2str(ret));
                char errBuff[AV_ERROR_MAX_STRING_SIZE];

                fprintf(stderr, "Error occurred: %s\n",
                        av_make_error_string(errBuff, AV_ERROR_MAX_STRING_SIZE, ret));
                return;
            }

        }


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
