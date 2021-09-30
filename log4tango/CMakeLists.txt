include(${CMAKE_CURRENT_SOURCE_DIR}/config/config.cmake)

add_subdirectory(include/log4tango)
add_subdirectory(src)

add_library(log4tango_objects OBJECT ${SOURCES})

if(WIN32)
    target_compile_definitions(log4tango_objects PRIVATE "${windows_defs}")
else()
    target_compile_options(log4tango_objects PRIVATE -fPIC)
endif()

set_target_properties(log4tango_objects
    PROPERTIES 
    PUBLIC_HEADER "${HEADERS}"
)

# In version previous 3.13 install must be done in the same directory
# And for 3.7 at least, we cannot use install on objects targets
if(${CMAKE_VERSION} VERSION_LESS "3.13.0")
    install(FILES ${HEADERS} DESTINATION
        ${CMAKE_INSTALL_FULL_INCLUDEDIR}/tango/log4tango)
endif()

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
