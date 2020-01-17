cmake_minimum_required(VERSION 3.4.1)
#event-lib

#libevent-2.1.8
set(event-dir ../../../../third_party/libevent)
set(curl-dir ../../../../third_party/curl)
set(openssl-dir ../../../../third_party/openssl)

include_directories(
        ../
        .
        ${event-dir}/include
        ${curl-dir}/include
        ${openssl-dir}/include)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} EVENT-SRC)


link_directories(
        ${event-dir}/${ANDROID_ABI}
        ${curl-dir}/${ANDROID_ABI}
        ${openssl-dir}/${ANDROID_ABI}
)

add_library(event-lib SHARED ${EVENT-SRC})

target_link_libraries(event-lib z log curl event crypto ssl)




