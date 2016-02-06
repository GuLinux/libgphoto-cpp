# - Find CIMG: finds the CIMG header-only library

FIND_PATH(CIMG_INCLUDE_DIRS CImg.h)
if(CIMG_INCLUDE_DIRS)
  set(CIMG_FOUND TRUE)
else(CIMG_INCLUDE_DIRS)
  set(CIMG_FOUND FALSE)
endif(CIMG_INCLUDE_DIRS)

find_package_handle_standard_args(CIMG REQUIRED_VARS CIMG_FOUND CIMG_INCLUDE_DIRS)
mark_as_advanced(CIMG_INCLUDE_DIRS)