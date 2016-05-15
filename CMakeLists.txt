cmake_minimum_required(VERSION 2.8)
PROJECT(gphoto++ CXX)
set(gphoto++_VERSION_MAJOR 0)
set(gphoto++_VERSION_MINOR 1)
set(gphoto++_VERSION_PATCH 0)
set(gphoto++_VERSION_STRING ${gphoto++_VERSION_MAJOR}.${gphoto++_VERSION_MINOR}.${gphoto++_VERSION_PATCH})

add_definitions(-DSOURCES_ROOT="${CMAKE_SOURCE_DIR}")

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(GPHOTO2 REQUIRED)
find_package(CImg REQUIRED)
include_directories(${CImg_INCLUDE_DIRS} gulinux-commons/)


set(GPHOTO_CPP_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src CACHE INTERNAL "libGPhotoC++ includes")

find_package(GTest)
SET(CMAKE_CXX_FLAGS "-Wall -std=c++11 -fpermissive" )
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++11 -fpermissive" )

add_subdirectory(src)
add_subdirectory(examples)
if(GTest_FOUND)
  add_subdirectory(tests)
endif(GTest_FOUND)
