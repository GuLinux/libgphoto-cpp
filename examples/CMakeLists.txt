include_directories(../src)
add_executable(shoot_preset_and_bulb shoot_preset_and_bulb.cpp)
target_link_libraries(shoot_preset_and_bulb gphoto++ ${libgphoto++_LIBRARIES})
