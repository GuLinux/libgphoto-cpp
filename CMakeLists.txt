cmake_minimum_required(VERSION 2.4.7)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(GPHOTO2 REQUIRED)
find_package(LibRaw REQUIRED)
find_package(JPEG REQUIRED)
find_package(CIMG REQUIRED)
find_package(GTest)
add_definitions(-Dcimg_display=0)
SET(CMAKE_CXX_FLAGS "-Wall -std=c++11 -fpermissive" )
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++11 -fpermissive" ) # TODO: remove -fpermissive



file(GLOB gphoto_widgets_SRCS widget*.cpp)
PROJECT(libgphoto++ CXX)
set(libgphoto++_SRCS 
  camerafile.cpp
  gphoto_wrapper.cpp
  logger.cpp
  read_jpeg_image.cpp
  read_raw_image.cpp
  serialshooter.cpp
  ${gphoto_widgets_SRCS}
)

add_library(gphoto++ STATIC ${libgphoto++_SRCS})
if(GTest_FOUND)
  add_subdirectory(tests)
endif(GTest_FOUND)
set(libgphoto++_LIBRARIES gphoto++ ${GPHOTO2_LIBRARY} ${GPHOTO2_PORT_LIBRARY} ${LIBRAW_LIBRARIES} ${JPEG_LIBRARIES} PARENT_SCOPE)