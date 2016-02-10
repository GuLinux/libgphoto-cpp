include_directories(${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE headers_FILES *.h)
add_subdirectory(widgets)
add_subdirectory(backend)
add_subdirectory(utils)
set(libgphoto++_SRCS 
  camerafile.cpp
  driver.cpp
  camera.cpp
  serialshooter.cpp
  $<TARGET_OBJECTS:gphoto_backend>
  $<TARGET_OBJECTS:gphoto_utils>
  $<TARGET_OBJECTS:gphoto_widgets>
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
