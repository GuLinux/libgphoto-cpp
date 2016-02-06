include_directories(..)
add_executable(test_gphoto_widgets test_gphoto_widgets.cpp)
target_link_libraries(test_gphoto_widgets gphoto++ ${GTEST_BOTH_LIBRARIES} gphoto2)
GTEST_ADD_TESTS(test_gphoto_widgets "" test_gphoto_widgets.cpp)

add_executable(gphoto_cli gphoto_cli.cpp)
target_link_libraries(gphoto_cli gphoto++ ${gphoto++_DEPS})