cmake_minimum_required(VERSION 3.4.1)
#cv-lib

include_directories(
        ../
        .
        ../../../../opencv/include)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} CV-SRC)

#opencv 4
set(opencv-dir ../../../../opencv)

link_directories(
        ${opencv-dir}/libs/${ANDROID_ABI}
        ${opencv-dir}/3rdparty/${ANDROID_ABI}
        ${opencv-dir}/staticlibs/${ANDROID_ABI}
)

set(opencv-lib
        #opencv
        opencv_calib3d
        opencv_core
        opencv_dnn
        opencv_features2d
        opencv_flann
        opencv_highgui
        opencv_imgcodecs
        opencv_imgproc
        opencv_ml
        opencv_objdetect
        opencv_photo
        opencv_stitching
        opencv_video
        opencv_videoio
        #opencv 3rdpart
        cpufeatures
        IlmImf
        ittnotify
        libjasper
        libjpeg-turbo
        libpng
        libprotobuf
        libtiff
        libwebp
        quirc
        tbb
        tegra_hal
        )

add_library(cv-lib SHARED ${CV-SRC})

target_link_libraries(cv-lib c++_shared log opencv_java4)

#${opencv-lib}



