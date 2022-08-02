add_subdirectory(idl)
add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(server)

set(TANGO_H_HEADER tango.h)
install(FILES ${TANGO_H_HEADER} DESTINATION "${CMAKE_INSTALL_FULL_INCLUDEDIR}/tango")
