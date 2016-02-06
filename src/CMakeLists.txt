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
