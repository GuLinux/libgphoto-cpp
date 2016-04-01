set(utils_SRCS logger.cpp read_raw_image.cpp read_jpeg_image.cpp)
add_library(gphoto_utils OBJECT ${utils_SRCS})