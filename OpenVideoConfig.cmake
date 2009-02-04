# - Configure the openvideo library
# Once done this will define
#
#  OPENVIDEO_FOUND - system has OPENVIDEO
#  OPENVIDEO_INCLUDE_DIRS - the OPENVIDEO include directories
#  OPENVIDEO_LIBRARIES - Link these to use OPENVIDEO
#  OPENVIDEO_LIBRARY_DIRS - link directories, useful for rpath
#

if(OPENVIDEO_INCLUDE_DIRS AND OPENVIDEO_LIBRARIES)

  # in cache already:
  set(OPENVIDEO_FOUND TRUE)

else(OPENVIDEO_INCLUDE_DIRS AND OPENVIDEO_LIBRARIES)

  get_filename_component(OPENVIDEO_ROOT "${CMAKE_CURRENT_LIST_FILE}" PATH)

  if(NOT EXISTS ${OPENVIDEO_ROOT}/CMakeLists.txt)
    # we are *not* in the source tree, so find the actual root from here:
    get_filename_component(OPENVIDEO_ROOT "${OPENVIDEO_ROOT}" PATH)
    get_filename_component(OPENVIDEO_ROOT "${OPENVIDEO_ROOT}" PATH)
  endif(NOT EXISTS ${OPENVIDEO_ROOT}/CMakeLists.txt)

  set(OPENVIDEO_INCLUDE_DIRS "${OPENVIDEO_ROOT}/include" CACHE STRING "OpenVideo include directory")
  set(OPENVIDEO_LIBRARIES "OpenVideo" CACHE STRING "OpenVideo library name")

  # search for the library (if we are in the source tree, it might not exist):
  find_library(OPENVIDEO_LIBRARY_PATH "${OPENVIDEO_LIBRARIES}" PATHS
    "${OPENVIDEO_ROOT}/lib"
    "${OPENVIDEO_ROOT}/lib/Release"
    "${OPENVIDEO_ROOT}/lib${LIBSUFFIX}"
    "${OPENVIDEO_ROOT}/lib64"
    "${OPENVIDEO_ROOT}/lib/win32"
    # make sure not to mix locally installed headers with globally installed binaries:
    NO_DEFAULT_PATH
    )

  if(OPENVIDEO_LIBRARY_PATH)
    # store library directory in cache:
    get_filename_component(OPENVIDEO_LIBRARY_DIRS "${OPENVIDEO_LIBRARY_PATH}" PATH)
    set(OPENVIDEO_LIBRARY_DIRS "${OPENVIDEO_LIBRARY_DIRS}" CACHE STRING "OpenVideo library directory")
    set(OPENVIDEO_FOUND TRUE)
  endif(OPENVIDEO_LIBRARY_PATH)

  if(OPENVIDEO_FOUND)
    if(NOT OpenVideo_FIND_QUIETLY)
      message(STATUS "Found OpenVideo: ${OPENVIDEO_LIBRARY_DIRS} ${OPENVIDEO_INCLUDE_DIRS}")
    endif(NOT OpenVideo_FIND_QUIETLY)
  else(OPENVIDEO_FOUND)
    if(OpenVideo_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find OpenVideo")
    endif(OpenVideo_FIND_REQUIRED)
  endif(OPENVIDEO_FOUND)

endif(OPENVIDEO_INCLUDE_DIRS AND OPENVIDEO_LIBRARIES)
