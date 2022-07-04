#TODO windows
if(NOT WIN32)
    SET(SCRIPTS
        run_with_fixture.sh
        setup_database.sh
        setup_devices.sh
        start_server.sh
        kill_server.sh
        tango_admin.sh
    )
else()
    SET(SCRIPTS
        run_with_fixture.ps1
        setup_database.ps1
    )
endif()

foreach(SCRIPT ${SCRIPTS})
    configure_file(${SCRIPT}.cmake ${SCRIPT} @ONLY)
endforeach(SCRIPT)
