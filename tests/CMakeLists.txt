include_directories(../src)
add_executable(test_gphoto_widgets test_gphoto_widgets.cpp)
target_link_libraries(test_gphoto_widgets gphoto++ ${GTEST_BOTH_LIBRARIES} gphoto2)
GTEST_ADD_TESTS(test_gphoto_widgets "" test_gphoto_widgets.cpp)

add_executable(test_exposure_parsing test_exposure_parsing.cpp)
target_link_libraries(test_exposure_parsing gphoto++ ${GTEST_BOTH_LIBRARIES} gphoto2)
GTEST_ADD_TESTS(test_exposure_parsing "" test_exposure_parsing.cpp)
