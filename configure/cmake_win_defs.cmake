#definitions and preprocessor
option(TANGO_INSTALL_DEPENDENCIES "Install the libraries that tango depends on" ON)

add_definitions(-D_WIN32)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
if(CMAKE_CL_64)
    add_definitions(-D_WIN64)
endif(CMAKE_CL_64)

if(MSVC14)
    add_definitions(-D_TIMERS_T_)
    add_definitions(-DHAVE_STRUCT_TIMESPEC)
    add_definitions(-D_TIMESPEC_DEFINED)
    add_definitions(-DSTRUCT_TIMESPEC_HAS_TV_SEC)
    add_definitions(-DSTRUCT_TIMESPEC_HAS_TV_NSEC)
endif(MSVC14)


list(APPEND static_defs "_LIB")
list(APPEND static_defs "_WINDOWS")
list(APPEND static_defs "_WINSTATIC")
list(APPEND static_defs "_TANGO_LIB")
list(APPEND static_defs "_MBCS")
list(APPEND static_defs "_CRT_SECURE_NO_DEPRECATE")
list(APPEND static_defs "JPG_USE_ASM")
list(APPEND static_defs "STATIC_EXPORT")
list(APPEND static_defs "OMNI_UNLOADABLE_STUBS")
list(APPEND static_defs "ZMQ_STATIC")
if(CMAKE_CL_64)
    list(REMOVE_ITEM static_defs "JPG_USE_ASM")
endif(CMAKE_CL_64)
message("static_defs: ${static_defs}")

list(APPEND dyn_defs "_LIB")
list(APPEND dyn_defs "_WINDOWS")
list(APPEND dyn_defs "LOG4TANGO_BUILD_DLL")
list(APPEND dyn_defs "_USRDLL")
list(APPEND dyn_defs "_TANGO_LIB")
list(APPEND dyn_defs "LOG4TANGO_HAS_DLL")
list(APPEND dyn_defs "_CRT_SECURE_NO_DEPRECATE")
list(APPEND dyn_defs "JPG_USE_ASM")
list(APPEND dyn_defs "OMNI_UNLOADABLE_STUBS")
if(CMAKE_CL_64)
    list(REMOVE_ITEM dyn_defs "JPG_USE_ASM")
endif(CMAKE_CL_64)
message("dyn_defs: ${dyn_defs}")

if(BUILD_SHARED_LIBS)
    message("selected dynamic-library build")
    set(windows_defs ${dyn_defs})
else()
    message("selected static-library build")
    set(windows_defs ${static_defs})
endif()
