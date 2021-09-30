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

list(APPEND TANGO_PUBLIC_HEADER
    $<TARGET_PROPERTY:client_objects,PUBLIC_HEADER>
    $<TARGET_PROPERTY:server_objects,PUBLIC_HEADER>
    )

set_target_properties(tango
    PROPERTIES
    PUBLIC_HEADER "${TANGO_PUBLIC_HEADER}"
    )

if(BUILD_SHARED_LIBS)
  target_compile_options(tango PRIVATE -fPIC)
  target_compile_definitions(tango PUBLIC _REENTRANT)
  set_target_properties(tango PROPERTIES
                        VERSION ${LIBRARY_VERSION}
                        SOVERSION ${SO_VERSION})
#  install(TARGETS tango LIBRARY DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}")
else()
#  install(TARGETS tango ARCHIVE DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}")
endif()

# Install Config -----------------------------------
include(CMakePackageConfigHelpers)

install(
    TARGETS tango
    EXPORT tangoTargets
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/tango
    LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})


# In version previous 3.13 install must be done in the same directory
if(${CMAKE_VERSION} VERSION_GREATER "3.13.0")
    install(
        TARGETS idl_objects
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/tango/idl
    )

    install(
        TARGETS log4tango_objects
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/tango/log4tango
    )
endif()

set(ConfigPackageLocation lib/cmake/tango)
set(Namespace tango-controls::)

# write tango version information
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/tango/tangoConfigVersion.cmake"
  VERSION ${LIBRARY_VERSION}
  COMPATIBILITY AnyNewerVersion
)

# export the targets built
export(EXPORT tangoTargets
  FILE
    "${CMAKE_CURRENT_BINARY_DIR}/tango/tangoTargets.cmake"
  NAMESPACE ${Namespace}
)

# generate the config file that includes the exports
configure_package_config_file(configure/tangoConfig.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/tango/tangoConfig.cmake"
  INSTALL_DESTINATION
    ${ConfigPackageLocation}
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

# install the exported targets
install(EXPORT tangoTargets
  FILE
    tangoTargets.cmake
  NAMESPACE
    ${Namespace}
  DESTINATION
    ${ConfigPackageLocation}
)

# install the cmake files
install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/tango/tangoConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/tango/tangoConfigVersion.cmake"
  DESTINATION
    ${ConfigPackageLocation}
)


configure_file(tango.pc.cmake tango.pc @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/tango.pc"
        DESTINATION "${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig")

include(configure/cpack_linux.cmake)
