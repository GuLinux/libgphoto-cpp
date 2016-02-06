cmake_minimum_required(VERSION 2.4.7)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(GPHOTO2 REQUIRED)
find_package(GTest)
SET(CMAKE_CXX_FLAGS "-Wall -std=c++11 -fpermissive" )
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++11 -fpermissive" ) # TODO: remove -fpermissive



file(GLOB gphoto_widgets_SRCS widget*.cpp)
PROJECT(libgphoto++ CXX)
set(libgphoto++_SRCS 
  camerafile.cpp
  gphoto_wrapper.cpp
  driver.cpp
  camera.cpp
  logger.cpp
  serialshooter.cpp
  exceptions.cpp
  ${gphoto_widgets_SRCS}
)

add_library(gphoto++ STATIC ${libgphoto++_SRCS})
set(gphoto++_DEPS ${GPHOTO2_LIBRARY} ${GPHOTO2_PORT_LIBRARY} pthread)
set(libgphoto++_LIBRARIES gphoto++ ${gphoto++_DEPS} PARENT_SCOPE)


if(GTest_FOUND)
  add_subdirectory(tests)
endif(GTest_FOUND)