#TODO windows
configure_file(tango_host.cmake tango_host @ONLY)

configure_file(pre_test.sh.cmake pre_test.sh @ONLY)
execute_process(COMMAND chmod +x ${CMAKE_CURRENT_BINARY_DIR}/pre_test.sh)
configure_file(post_test.sh.cmake post_test.sh @ONLY)
execute_process(COMMAND chmod +x ${CMAKE_CURRENT_BINARY_DIR}/post_test.sh)

execute_process(COMMAND chmod +x ${CMAKE_CURRENT_SOURCE_DIR}/setup.sh)
add_custom_target(start-tango ${CMAKE_CURRENT_SOURCE_DIR}/setup.sh
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        SOURCES setup.sh)

configure_file(run.sh.cmake run.sh @ONLY)
execute_process(COMMAND chmod +x ${CMAKE_CURRENT_BINARY_DIR}/run.sh)
add_custom_target(run-tests ${CMAKE_CURRENT_BINARY_DIR}/run.sh
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        SOURCES run.sh.cmake)

execute_process(COMMAND chmod +x ${CMAKE_CURRENT_SOURCE_DIR}/shutdown.sh)
add_custom_target(stop-tango ${CMAKE_CURRENT_SOURCE_DIR}/shutdown.sh
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        SOURCES shutdown.sh)
