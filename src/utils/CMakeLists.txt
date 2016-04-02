add_definitions(-Dcimg_display=0)

set(utils_SRCS logger.cpp read_image.cpp read_raw_image.cpp read_jpeg_image.cpp)
add_library(gphoto_utils OBJECT ${utils_SRCS})