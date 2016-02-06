cmake_minimum_required(VERSION 2.4.7)
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

add_library(libgphoto++ STATIC ${libgphoto++_SRCS})
