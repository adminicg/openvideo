ADD_EXECUTABLE(openvideo-bin main.cxx)
TARGET_LINK_LIBRARIES(openvideo-bin openvideo)
INSTALL(TARGETS openvideo-bin DESTINATION ${CMAKE_INSTALL_BINDIR})
