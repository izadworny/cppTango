project(libtango)

add_library(tango $<TARGET_OBJECTS:log4tango_objects>
                  $<TARGET_OBJECTS:client_objects>
                  $<TARGET_OBJECTS:idl_objects>
                  $<TARGET_OBJECTS:jpeg_objects>
                  $<TARGET_OBJECTS:jpeg_mmx_objects>
                  $<TARGET_OBJECTS:server_objects>)
target_link_libraries(tango PUBLIC ${ZMQ_PKG_LIBRARIES} ${OMNIORB_PKG_LIBRARIES} ${OMNICOS_PKG_LIBRARIES} ${OMNIDYN_PKG_LIBRARIES} ${CMAKE_DL_LIBS})
target_include_directories(tango SYSTEM PUBLIC ${ZMQ_PKG_INCLUDE_DIRS} ${OMNIORB_PKG_INCLUDE_DIRS} ${OMNIDYN_PKG_INCLUDE_DIRS})
target_compile_options(tango PUBLIC ${ZMQ_PKG_CFLAGS_OTHER} ${OMNIORB_PKG_CFLAGS_OTHER} ${OMNICOS_PKG_CFLAGS_OTHER} ${OMNIDYN_PKG_CFLAGS_OTHER})

set_target_properties(
    log4tango_objects client_objects idl_objects
    jpeg_objects jpeg_mmx_objects server_objects
    PROPERTIES
    UNITY_BUILD_CODE_BEFORE_INCLUDE "// NOLINTNEXTLINE(bugprone-suspicious-include)")

if(BUILD_SHARED_LIBS)
  target_compile_options(tango PRIVATE -fPIC)
  target_compile_definitions(tango PUBLIC _REENTRANT)
  set_target_properties(tango PROPERTIES
                        VERSION ${LIBRARY_VERSION}
                        SOVERSION ${SO_VERSION})
  install(TARGETS tango LIBRARY DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}")
else()
  install(TARGETS tango ARCHIVE DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}")
endif()

configure_file(tango.pc.cmake tango.pc @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/tango.pc"
        DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig")

include(configure/cpack_linux.cmake)
