file(GLOB gphoto_widgets_SRCS widget*.cpp)
file(GLOB headers_FILES *.h)
PROJECT(libgphoto++ CXX)
set(libgphoto++_SRCS 
  camerafile.cpp
  gphoto_wrapper.cpp
  driver.cpp
  camera.cpp
  logger.cpp
  serialshooter.cpp
  exceptions.cpp
  list.cpp
  ${gphoto_widgets_SRCS}
)
option(BUILD_SHARED "Build shared library" Off)

if(BUILD_SHARED)
  add_library(gphoto++ SHARED ${libgphoto++_SRCS})
else(BUILD_SHARED)
  add_library(gphoto++ STATIC ${libgphoto++_SRCS})
endif(BUILD_SHARED)

set_target_properties(gphoto++ PROPERTIES VERSION ${gphoto++_VERSION_STRING} SOVERSION ${gphoto++_VERSION_MAJOR})

set(gphoto++_DEPS ${GPHOTO2_LIBRARY} ${GPHOTO2_PORT_LIBRARY} pthread)
set(libgphoto++_LIBRARIES gphoto++ ${gphoto++_DEPS} PARENT_SCOPE)
install(TARGETS gphoto++ DESTINATION lib)
install(FILES ${headers_FILES} DESTINATION includes/gphoto++)
