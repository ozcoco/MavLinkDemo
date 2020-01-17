cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_STANDARD 17)

#cv-lib
message(STATUS "build cv-lib ------>")
add_subdirectory(cv)

#av-lib
message(STATUS "build av-lib ------>")
add_subdirectory(av)

#event-lib
message(STATUS "build av-lib ------>")
add_subdirectory(event)