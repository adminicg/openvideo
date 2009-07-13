project(openvideo)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true) # Avoid unneccesary code
cmake_minimum_required(VERSION 2.6)

file(TO_CMAKE_PATH "$ENV{ICG_CMAKE_MODULE_PATH}" OT_CMAKE_MODULE_PATH)
set(CMAKE_MODULE_PATH ${OT_CMAKE_MODULE_PATH})

# specify the lib and dll output folders for win32
if(WIN32)
   set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${openvideo_SOURCE_DIR}/bin/win32)
   set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${openvideo_SOURCE_DIR}/lib/win32)
   set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${openvideo_SOURCE_DIR}/lib/win32)
endif(WIN32)

if(UNIX)
   include(UsePkgConfig)
endif(UNIX)

find_package(ACE REQUIRED)
find_package(TinyXMLMod REQUIRED)
find_package(OpenGL REQUIRED)

if(WIN32)
   ## this is necessary for FindGLUT to work correctly under win32
   #set(GLUT_ROOT_PATH $ENV{GLUT_ROOT_PATH}) -> only for original FindGLUT!
   find_package(GLUT_ICG) # use the ICG own FindGLUT version
else(WIN32)
   find_package(GLUT)   
endif(WIN32)
if(GLUT_FOUND)
   option(OT_USE_GLUT "Build with GLUT support?" ON)
else(GLUT_FOUND)
   option(OT_USE_GLUT "Build with GLUT support?" OFF)
endif(GLUT_FOUND)

if(WIN32)
   #find_package(DSVL)
   if(DSVL_FOUND)
      option(OT_USE_DSVL "Build with Windows DirectShow Video Library support?" ON)
   else(DSVL_FOUND)
      option(OT_USE_DSVL "Build with Windows DirectShow Video Library support?" OFF)
   endif(DSVL_FOUND)
endif(WIN32)

find_package(OpenCV)
if(OpenCV_FOUND)
   option(OT_USE_OPENCV "Build with OpenCV support?" ON)
else(OpenCV_FOUND)
   option(OT_USE_OPENCV "Build with OpenCV support?" OFF)
endif(OpenCV_FOUND)

find_package(AVIFile64)
if(AVIFILE_FOUND)
   if(UNIX)
      option(OT_USE_AVIFILE64 "Build with AVIFile64 support (not on Windows!)" ON)
   else(UNIX)
      option(OT_USE_AVIFILE64 "Build with AVIFile64 support (not on Windows!)" OFF)
   endif(UNIX)
else(AVIFILE_FOUND)
   option(OT_USE_AVIFILE64 "Build with AVIFile64 support (not on Windows!)" OFF)
endif(AVIFILE_FOUND)


if(UNIX)
  add_definitions(-DLINUX)
  add_definitions(-DENABLE_VIDEOSINK)
endif(UNIX)

set(CPACK_GENERATOR "TBZ2")
set(CPACK_SOURCE_GENERATOR "TBZ2")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
set(CPACK_PACKAGE_NAME "openvideo")

# RPM variables:
if(NOT CMAKE_INSTALL_BINDIR)
set(CMAKE_INSTALL_BINDIR ${CMAKE_INSTALL_PREFIX}/bin)
endif(NOT CMAKE_INSTALL_BINDIR)
if(NOT CMAKE_INSTALL_LIBDIR)
set(CMAKE_INSTALL_LIBDIR ${CMAKE_INSTALL_PREFIX}/lib)
endif(NOT CMAKE_INSTALL_LIBDIR)
if(NOT CMAKE_INSTALL_DATADIR)
set(CMAKE_INSTALL_DATADIR ${CMAKE_INSTALL_PREFIX}/share)
endif(NOT CMAKE_INSTALL_DATADIR)
if(NOT CMAKE_INSTALL_DOCDIR)
set(CMAKE_INSTALL_DOCDIR ${CMAKE_INSTALL_DATADIR}/doc/packages)
endif(NOT CMAKE_INSTALL_DOCDIR)
if(NOT CMAKE_INSTALL_MANDIR)
set(CMAKE_INSTALL_MANDIR ${CMAKE_INSTALL_DATADIR}/man)
endif(NOT CMAKE_INSTALL_MANDIR)

if(UNIX)
   # configure openvideo.pc:
   set(OPENVIDEO_PREFIX ${CMAKE_INSTALL_PREFIX})
   set(OPENVIDEO_LIBDIR ${CMAKE_INSTALL_LIBDIR})
   set(OPENVIDEO_PROJECT_NAME ${CPACK_PACKAGE_NAME})
   set(OPENVIDEO_PROJECT_VERSION ${CPACK_PACKAGE_VERSION})
   set(OPENVIDEO_PROJECT_DESCRIPTION "Support for video data")
   set(OPENVIDEO_PROJECT_LIBNAME ${CPACK_PACKAGE_NAME})
   set(OPENVIDEO_PROJECT_DEFINES "")
   
   configure_file(openvideo.pc.in openvideo.pc @ONLY)
   install(FILES ${CMAKE_BINARY_DIR}/openvideo.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
   configure_file(openvideo.spec.in openvideo.spec)
endif(UNIX)

#include_directories(include ${OPENGL_INCLUDE_DIR} ${GLUT_INCLUDE_DIR})

if(WIN32)
   add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
   add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
   add_definitions(-D_CRT_SECURE_NO_DEPRECATE_GLOBALS)
   set(CMAKE_DEBUG_POSTFIX "d")
endif(WIN32)


# ACE
include_directories(${ACE_INCLUDE_DIR})
link_directories(${ACE_LIBRARY_DIR})

# TinyXMLMod
include_directories(${TINY_XML_MOD_INCLUDE_DIR})
link_directories(${TINY_XML_MOD_LIBRARY_DIR})
add_definitions(${TINY_XML_MOD_DEFINITIONS})

# GLUT
if(OT_USE_GLUT)
   include_directories(${GLUT_INCLUDE_DIR})
endif(OT_USE_GLUT)

# OpenCV
if(OT_USE_OPENCV)
   include_directories(${OPENCV_INCLUDE_DIRS})
   link_directories(${OpenCV_LINK_DIRECTORIES})
endif(OT_USE_OPENCV)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(standalone)

install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_PREFIX} PATTERN ".svn" EXCLUDE)
install(FILES OpenVideoConfig.cmake DESTINATION lib/OpenVideo)

# packaging:

set(CPACK_IGNORE_FILES
  ".depend"
  ".scon*"
  "/#.*#$"
  "/CVS/"
  "/log"
  "/trunk/build/"
  "CMakeCache\\\\.txt"
  "CMakeFiles"
  "CPack.*Config\\\\.cmake"
  "Makefile"
  "SCons*"
  "\\\\.#"
  "\\\\.a$"
  "\\\\.aux$"
  "\\\\.bz2$"
  "\\\\.dsp$"
  "\\\\.dvi$"
  "\\\\.eps$"
  "\\\\.gz$"
  "\\\\.iv$"
  "\\\\.log$"
  "\\\\.o$"
  "\\\\.out"
  "\\\\.pgm$"
  "\\\\.pro$"
  "\\\\.ps$"
  "\\\\.save"
  "\\\\.so"
  "\\\\.svn"
  "\\\\.swp$"
  "\\\\.tbz$"
  "\\\\.tex$"
  "\\\\.tgz$"
  "\\\\.vcproj$"
  "^/box.cpp"
  "_CPack_Packages"
  "a.out"
  "cgparams$"
  "cmake_install.cmake"
  "cvs_import"
  "install.sh"
  "install_manifest.txt"
  "makedist.sh"
  "moc_"
  "pkgconfig"
  "progress.make"
  "~$"
)

set(CPACK_SOURCE_IGNORE_FILES ${CPACK_IGNORE_FILES})
set(CPACK_SOURCE_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION})
include(CPack)
add_custom_target(dist COMMAND make clean && make package_source)
add_custom_target(clean-all COMMAND find \\\( -name CMakeFiles -or -name CMakeCache.txt -or -name Makefile -or -name '*.cmake' -or -name _CPack_Packages -or -name '*~' \\\) -prune -exec rm -rf {} '\;')
