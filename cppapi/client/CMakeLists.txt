set(SOURCES dbapi_class.cpp
            dbapi_server.cpp
            dbapi_datum.cpp
            dbapi_base.cpp
            dbapi_device.cpp
            dbapi_history.cpp
            dbapi_attribute.cpp
            dbapi_cache.cpp
            dbapi_serverdata.cpp
            devapi_attr.cpp
            devapi_base.cpp
            devapi_data.cpp
            devapi_datahist.cpp
            devapi_utils.cpp
            devapi_pipe.cpp
            api_util.cpp
            asynreq.cpp
            cbthread.cpp
            proxy_asyn.cpp
            proxy_asyn_cb.cpp
            attr_proxy.cpp
            group.cpp
            filedatabase.cpp
            apiexcept.cpp
            accessproxy.cpp
            lockthread.cpp
            event.cpp
            eventkeepalive.cpp
            eventqueue.cpp
            notifdeventconsumer.cpp
            zmqeventconsumer.cpp)

set(HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/accessproxy.h
            ${CMAKE_CURRENT_SOURCE_DIR}/apiexcept.h
            ${CMAKE_CURRENT_SOURCE_DIR}/cbthread.h
            ${CMAKE_CURRENT_SOURCE_DIR}/dbapi.h
            ${CMAKE_CURRENT_SOURCE_DIR}/devapi.h
            ${CMAKE_CURRENT_SOURCE_DIR}/devasyn.h
            ${CMAKE_CURRENT_SOURCE_DIR}/event.h
            ${CMAKE_CURRENT_SOURCE_DIR}/eventconsumer.h
            ${CMAKE_CURRENT_SOURCE_DIR}/filedatabase.h
            ${CMAKE_CURRENT_SOURCE_DIR}/group.h
            ${CMAKE_CURRENT_SOURCE_DIR}/lockthread.h
            ${CMAKE_CURRENT_SOURCE_DIR}/Database.h
            ${CMAKE_CURRENT_SOURCE_DIR}/DbDevice.h
            ${CMAKE_CURRENT_SOURCE_DIR}/ApiUtil.h
            ${CMAKE_CURRENT_SOURCE_DIR}/DeviceData.h
            ${CMAKE_CURRENT_SOURCE_DIR}/DeviceAttribute.h
            ${CMAKE_CURRENT_SOURCE_DIR}/DevicePipe.h
            ${CMAKE_CURRENT_SOURCE_DIR}/Connection.h
            ${CMAKE_CURRENT_SOURCE_DIR}/DeviceProxy.h
            ${CMAKE_CURRENT_SOURCE_DIR}/AttributeProxy.h
            ${CMAKE_CURRENT_SOURCE_DIR}/event.tpp
            ${CMAKE_CURRENT_SOURCE_DIR}/devapi_attr.tpp
            ${CMAKE_CURRENT_SOURCE_DIR}/devapi_utils.tpp
            ${CMAKE_CURRENT_SOURCE_DIR}/api_util.tpp
            ${CMAKE_CURRENT_SOURCE_DIR}/devapi_pipe.tpp)

add_subdirectory(helpers)

        #list(APPEND HEADERS ${HELPERS})

add_library(client_objects OBJECT ${SOURCES})
add_dependencies(client_objects idl_objects)

if(WIN32)
    target_compile_definitions(client_objects PRIVATE "${windows_defs}")

    if(USE_PCH AND NOT BUILD_SHARED_LIBS)
        # shared cannot be compiled with PCH due to __@_PchSym_ collision
        tango_target_pch(client_objects)
    endif()
else()
    target_compile_options(client_objects PRIVATE -fPIC)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_definitions(client_objects PRIVATE _REENTRANT _TANGO_LIB)
    else()
        # Do not define _TANGO_LIB when compiling Tango debug library on Linux
        # in order to keep the same behaviour as in the past:
        # cout messages are not redirected to the logging system but are
        # instead displayed directly on the console
        target_compile_definitions(client_objects PRIVATE _REENTRANT)
    endif()
    if(USE_PCH)
        tango_target_pch(client_objects)
    endif()
endif()
set_target_properties(client_objects
    PROPERTIES
    PUBLIC_HEADER "${HEADERS};${HELPERS}"
)
