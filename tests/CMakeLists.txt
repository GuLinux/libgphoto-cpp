include_directories(..)
add_definitions(-DIMAGE_SAMPLES="\\"${CMAKE_CURRENT_SOURCE_DIR}/image_samples\\"")
add_executable(test_raw_image_reader test_raw_image_reader.cpp  original_implementation/gphoto_readimage.c )
target_link_libraries(test_raw_image_reader gphoto++ ${GTEST_BOTH_LIBRARIES} raw jpeg cfitsio)
GTEST_ADD_TESTS(test_raw_image_reader "" test_raw_image_reader.cpp)

add_executable(test_jpeg_image_reader test_jpeg_image_reader.cpp original_implementation/gphoto_readimage.c)
target_link_libraries(test_jpeg_image_reader gphoto++ ${GTEST_BOTH_LIBRARIES} jpeg cfitsio pthread)
GTEST_ADD_TESTS(test_jpeg_image_reader "" test_jpeg_image_reader.cpp)

add_executable(test_gphoto_widgets test_gphoto_widgets.cpp)
target_link_libraries(test_gphoto_widgets gphoto++ ${GTEST_BOTH_LIBRARIES} gphoto2)
GTEST_ADD_TESTS(test_gphoto_widgets "" test_gphoto_widgets.cpp)