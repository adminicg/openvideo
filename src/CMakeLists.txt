file(GLOB_RECURSE OPENVIDEO_SOURCES *.c *.cpp *.cxx)
file(GLOB_RECURSE OPENVIDEO_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ../include/*.h ../include/*.hpp)

if(OT_USE_AVIFILE64)
   add_definitions("-DOT_USE_AVIFILE64")
endif(OT_USE_AVIFILE64)

add_definitions(-DTINYXML_MOD)
add_definitions(-DENABLE_GL_TEXTURE_2D_SINK)
add_definitions(-DENABLE_VIDEOSINK)

if(OT_USE_DSVL)
   add_definitions(-DENABLE_DSVLSRC)
endif(OT_USE_DSVL)

if(OT_USE_GLUT)
   #message("Compile in GLUT sink support")
   add_definitions(-DENABLE_GLUTSINK)
endif(OT_USE_GLUT)
if(OT_USE_OPENCV)
   #message("Compile in OpenCV support")
   add_definitions(-DENABLE_OPENCV)
   include_directories(${OpenCV_INCLUDE_DIRS})
   link_directories(${OpenCV_LINK_DIRECTORIES})
endif(OT_USE_OPENCV)

if(WIN32)
   add_definitions(-DOT_DO_NOT_LINK_TINYXML_VIA_PRAGMA_COMMENT_LIB) # This is a workaround for win32
   add_definitions(-DOPENVIDEO_EXPORTS)
endif(WIN32)

add_library(openvideo SHARED ${OPENVIDEO_SOURCES} ${OPENVIDEO_HEADERS})
target_link_libraries(openvideo ${ACE_LIBRARIES} ${TINY_XML_MOD_LIBRARIES} ${AVIFILE_LIBRARIES})
if(OT_USE_GLUT)
   target_link_libraries(openvideo ${GLUT_LIBRARIES})
endif(OT_USE_GLUT)
if(OT_USE_OPENCV)
   target_link_libraries(openvideo ${OpenCV_LIBRARIES})
endif(OT_USE_OPENCV)
   
install(TARGETS openvideo DESTINATION ${CMAKE_INSTALL_LIBDIR})
set_target_properties(openvideo PROPERTIES VERSION ${CPACK_PACKAGE_VERSION} SOVERSION ${CPACK_PACKAGE_VERSION_MAJOR})
