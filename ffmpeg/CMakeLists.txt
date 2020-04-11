#

#ffmpeg 4.2.2
set(ffmpeg-dir ../../../../ffmpeg_4_2_2)

include_directories(
        ../
        .
        ${ffmpeg-dir}/${ANDROID_ABI}/include)

link_directories(
        ${ffmpeg-dir}/${ANDROID_ABI}
)


set(ffmpeg-lib
        avcodec
        avdevice
        avfilter
        avformat
        avutil
        postproc
        swresample
        swscale)


set(SRC cmdutils.c cmdutils.h config.h ffmpeg.c
        ffmpeg.h ffmpeg-exec.h ffmpeg-exec-jni.cpp
        ffmpeg_filter.c ffmpeg_hw.c
        ffmpeg_opt.c)

add_library(ffmpeg-exec SHARED ${SRC})

target_link_libraries(ffmpeg-exec log ${ffmpeg-lib})