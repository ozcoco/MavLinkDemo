cmake_minimum_required(VERSION 3.4.1)
#cv-lib

include_directories(
        ../
        .
        ../../../../ffmpeg/include)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} AV-SRC)

#ffmpeg 4.0.1
set(ffmpeg-dir ../../../../ffmpeg)

link_directories(
        ${ffmpeg-dir}/${ANDROID_ABI}
)

set(ffmpeg-lib
        avcodec-58
        avdevice-58
        avfilter-7
        avformat-58
        avutil-56
        postproc-55
        swresample-3
        swscale-5)

add_library(av-lib SHARED ${AV-SRC})

target_link_libraries(av-lib android jnigraphics m log ${ffmpeg-lib})



