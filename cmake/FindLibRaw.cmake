# - Try to find LIBRAW
# Once done this will define
#
#  LIBRAW_FOUND - system has LIBRAW
#  LIBRAW_INCLUDE_DIR - the LIBRAW include directory
#  LIBRAW_LIBRARIES - Link these to use LIBRAW
#  LIBRAW_DRIVER_LIBRARIES - Link to these to build LIBRAW drivers with indibase support
#  LIBRAW_CLIENT_LIBRARIES - Link to these to build LIBRAW clients
#  LIBRAW_DATA_DIR - LIBRAW shared data dir.

# Copyright (c) 2016, Marco Gulino <marco@gulinux.net>
# Based on FindINDI
# Copyright (c) 2015, Jasem Mutlaq <mutlaqja@ikarustech.com>
# Copyright (c) 2012, Pino Toscano <pino@kde.org>
# Based on FindLibfacile by Carsten Niehaus, <cniehaus@gmx.de>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

if (LIBRAW_INCLUDE_DIR AND LIBRAW_DATA_DIR AND LIBRAW_LIBRARIES)

  # in cache already
  set(LIBRAW_FOUND TRUE)
  message(STATUS "Found LIBRAW: ${LIBRAW_LIBRARIES}")


else (LIBRAW_INCLUDE_DIR AND LIBRAW_DATA_DIR AND LIBRAW_LIBRARIES)

  find_package(PkgConfig)

  if (PKG_CONFIG_FOUND)
    if (LIBRAW_FIND_VERSION)
      set(version_string ">=${LIBRAW_FIND_VERSION}")
    endif()
    pkg_check_modules(PC_LIBRAW libraw${version_string})
  else()
    # assume it was found
    set(PC_LIBRAW_FOUND TRUE)
  endif()

  if (PC_LIBRAW_FOUND)
    find_path(LIBRAW_INCLUDE_DIR libraw.h
      PATH_SUFFIXES libraw
      HINTS ${PC_LIBRAW_INCLUDE_DIRS}
    )

    find_library(LIBRAW_LIBRARIES NAMES raw
      HINTS ${PC_LIBRAW_LIBRARY_DIRS}
    )

    set(LIBRAW_VERSION "${PC_LIBRAW_VERSION}")

  endif()

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(LIBRAW
                                    REQUIRED_VARS LIBRAW_INCLUDE_DIR LIBRAW_LIBRARIES 
                                    VERSION_VAR LIBRAW_VERSION
  )

  mark_as_advanced(LIBRAW_INCLUDE_DIR LIBRAW_DATA_DIR LIBRAW_LIBRARIES )

endif (LIBRAW_INCLUDE_DIR AND LIBRAW_DATA_DIR AND LIBRAW_LIBRARIES)
