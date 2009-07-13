add_executable(openvideo-bin main.cxx)
target_link_libraries(openvideo-bin openvideo)
install(TARGETS openvideo-bin DESTINATION ${CMAKE_INSTALL_BINDIR})
