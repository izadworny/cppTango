set(SOURCES attrdesc.cpp
            attrgetsetprop.cpp
            attribute.cpp
            attrsetval.cpp
            attrmanip.cpp
            basiccommand.cpp
            blackbox.cpp
            class_factory.cpp
            classattribute.cpp
            command.cpp
            coutappender.cpp
            classpipe.cpp
            dev_event.cpp
            dev_poll.cpp
            device.cpp
            device_2.cpp
            device_3.cpp
            device_4.cpp
            device_5.cpp
            deviceclass.cpp
            devicelog.cpp
            devintr.cpp
            dintrthread.cpp
            dserver.cpp
            dserverclass.cpp
            dserverlock.cpp
            dserverlog.cpp
            dserverpoll.cpp
            dserversignal.cpp
            encoded_attribute.cpp
            eventcmds.cpp
            eventsupplier.cpp
            except.cpp
            fwdattrdesc.cpp
            fwdattribute.cpp
            logcmds.cpp
            logging.cpp
            logstream.cpp
            multiattribute.cpp
            notifdeventsupplier.cpp
            pipe.cpp
            pollcmds.cpp
            pollobj.cpp
            pollring.cpp
            pollthread.cpp
            rootattreg.cpp
            seqvec.cpp
            subdev_diag.cpp
            tangoappender.cpp
            tangorollingfileappender.cpp
            templ_inst.cpp
            thsig.cpp
            utils.cpp
            utils_polling.cpp
            utils_shut.cpp
            w_attribute.cpp
            w_pipe.cpp
            zmqeventsupplier.cpp)

set(HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/attrdesc.h
            ${CMAKE_CURRENT_SOURCE_DIR}/attribute.h
            ${CMAKE_CURRENT_SOURCE_DIR}/attribute.tpp
            ${CMAKE_CURRENT_SOURCE_DIR}/attrsetval.tpp
            ${CMAKE_CURRENT_SOURCE_DIR}/attribute_spec.tpp
            ${CMAKE_CURRENT_SOURCE_DIR}/attrmanip.h
            ${CMAKE_CURRENT_SOURCE_DIR}/attrprop.h
            ${CMAKE_CURRENT_SOURCE_DIR}/attrprop.tpp
            ${CMAKE_CURRENT_SOURCE_DIR}/auto_tango_monitor.h
            ${CMAKE_CURRENT_SOURCE_DIR}/basiccommand.h
            ${CMAKE_CURRENT_SOURCE_DIR}/blackbox.h
            ${CMAKE_CURRENT_SOURCE_DIR}/classattribute.h
            ${CMAKE_CURRENT_SOURCE_DIR}/classpipe.h
            ${CMAKE_CURRENT_SOURCE_DIR}/command.h
            ${CMAKE_CURRENT_SOURCE_DIR}/pipe.h
            ${CMAKE_CURRENT_SOURCE_DIR}/pipe.tpp
            ${CMAKE_CURRENT_SOURCE_DIR}/coutappender.h
            ${CMAKE_CURRENT_SOURCE_DIR}/coutbuf.h
            ${CMAKE_CURRENT_SOURCE_DIR}/device.h
            ${CMAKE_CURRENT_SOURCE_DIR}/device_2.h
            ${CMAKE_CURRENT_SOURCE_DIR}/device_3.h
            ${CMAKE_CURRENT_SOURCE_DIR}/device_3.tpp
            ${CMAKE_CURRENT_SOURCE_DIR}/device_4.h
            ${CMAKE_CURRENT_SOURCE_DIR}/device_5.h
            ${CMAKE_CURRENT_SOURCE_DIR}/deviceclass.h
            ${CMAKE_CURRENT_SOURCE_DIR}/devintr.h
            ${CMAKE_CURRENT_SOURCE_DIR}/dintrthread.h
            ${CMAKE_CURRENT_SOURCE_DIR}/dserver.h
            ${CMAKE_CURRENT_SOURCE_DIR}/dserverclass.h
            ${CMAKE_CURRENT_SOURCE_DIR}/dserversignal.h
            ${CMAKE_CURRENT_SOURCE_DIR}/eventsupplier.h
            ${CMAKE_CURRENT_SOURCE_DIR}/except.h
            ${CMAKE_CURRENT_SOURCE_DIR}/fwdattrdesc.h
            ${CMAKE_CURRENT_SOURCE_DIR}/fwdattribute.h
            ${CMAKE_CURRENT_SOURCE_DIR}/fwdattribute.tpp
            ${CMAKE_CURRENT_SOURCE_DIR}/fwdattribute_spec.tpp
            ${CMAKE_CURRENT_SOURCE_DIR}/log4tango.h
            ${CMAKE_CURRENT_SOURCE_DIR}/logcmds.h
            ${CMAKE_CURRENT_SOURCE_DIR}/logging.h
            ${CMAKE_CURRENT_SOURCE_DIR}/logstream.h
            ${CMAKE_CURRENT_SOURCE_DIR}/multiattribute.h
            ${CMAKE_CURRENT_SOURCE_DIR}/ntservice.h
            ${CMAKE_CURRENT_SOURCE_DIR}/pipedesc.h
            ${CMAKE_CURRENT_SOURCE_DIR}/pollcmds.h
            ${CMAKE_CURRENT_SOURCE_DIR}/pollext.h
            ${CMAKE_CURRENT_SOURCE_DIR}/pollext.tpp
            ${CMAKE_CURRENT_SOURCE_DIR}/pollobj.h
            ${CMAKE_CURRENT_SOURCE_DIR}/pollring.h
            ${CMAKE_CURRENT_SOURCE_DIR}/pollring.tpp
            ${CMAKE_CURRENT_SOURCE_DIR}/pollthread.h
            ${CMAKE_CURRENT_SOURCE_DIR}/pollthread.tpp
            ${CMAKE_CURRENT_SOURCE_DIR}/readers_writers_lock.h
            ${CMAKE_CURRENT_SOURCE_DIR}/rootattreg.h
            ${CMAKE_CURRENT_SOURCE_DIR}/seqvec.h
            ${CMAKE_CURRENT_SOURCE_DIR}/tango.h
            ${CMAKE_CURRENT_SOURCE_DIR}/tango_config.h
            ${CMAKE_CURRENT_SOURCE_DIR}/tango_monitor.h
            ${CMAKE_CURRENT_SOURCE_DIR}/tangoappender.h
            ${CMAKE_CURRENT_SOURCE_DIR}/tangorollingfileappender.h
            ${CMAKE_CURRENT_SOURCE_DIR}/utils.h
            ${CMAKE_CURRENT_SOURCE_DIR}/utils.tpp
            ${CMAKE_CURRENT_SOURCE_DIR}/utils_spec.tpp
            ${CMAKE_CURRENT_SOURCE_DIR}/w32win.h
            ${CMAKE_CURRENT_SOURCE_DIR}/w_attribute.h
            ${CMAKE_CURRENT_SOURCE_DIR}/w_attribute.tpp
            ${CMAKE_CURRENT_SOURCE_DIR}/w_attrsetval.tpp
            ${CMAKE_CURRENT_SOURCE_DIR}/w_attribute_spec.tpp
            ${CMAKE_CURRENT_SOURCE_DIR}/w_pipe.h
            ${CMAKE_CURRENT_SOURCE_DIR}/w_pipe.tpp
            ${CMAKE_CURRENT_SOURCE_DIR}/subdev_diag.h
            ${CMAKE_CURRENT_SOURCE_DIR}/encoded_attribute.h
            ${CMAKE_CURRENT_SOURCE_DIR}/encoded_format.h
            ${CMAKE_CURRENT_SOURCE_DIR}/event_subscription_state.h
            ${CMAKE_CURRENT_SOURCE_DIR}/tango_optional.h
            ${CMAKE_CURRENT_SOURCE_DIR}/tango_clock.h
            )

add_subdirectory(idl)
add_subdirectory(jpeg)
add_subdirectory(jpeg_mmx)

if(WIN32)
    set(SOURCES_WIN
        coutbuf.cpp
        ntservice.cpp
        w32win.cpp)

    list(APPEND SOURCES ${SOURCES_WIN})
endif()

add_library(server_objects OBJECT ${SOURCES})
add_dependencies(server_objects idl_objects)

if(WIN32)
    target_compile_definitions(server_objects PRIVATE "${windows_defs}")

    if(USE_PCH)
        tango_target_pch(server_objects)
    endif()
else()
    target_compile_options(server_objects PRIVATE -fPIC)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_definitions(server_objects PRIVATE _REENTRANT _TANGO_LIB)
    else()
        # Do not define _TANGO_LIB when compiling Tango debug library on Linux
        # in order to keep the same behaviour as in the past:
        # A Linux Tango Device Server using the debug tango lib will display
        # twice "Ready to accept requests" at startup
        target_compile_definitions(server_objects PRIVATE _REENTRANT)
    endif()
    if(USE_PCH)
        tango_add_pch(tango_pch server_objects "")
        tango_target_pch(server_objects)
    endif()
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tango_const.h.in ${CMAKE_CURRENT_BINARY_DIR}/tango_const.h)

list(APPEND HEADERS ${CMAKE_CURRENT_BINARY_DIR}/tango_const.h)

set_target_properties(server_objects
    PROPERTIES
    PUBLIC_HEADER "${HEADERS}"
)
