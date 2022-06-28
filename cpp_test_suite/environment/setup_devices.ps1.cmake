if($TANGO_TEST_CASE_DIRECTORY -ne $null)
{
	$env:TANGO_TEST_CASE_DIRECTORY=$TANGO_TEST_CASE_DIRECTORY
}
else
{
	$env:TANGO_TEST_CASE_DIRECTORY="."
}

&{@PROJECT_BINARY_DIR@/new_tests/Release/conf_devtest.exe @SERV_NAME@/@INST_NAME@ `
    @DEV1@ `
    @DEV2@ `
    @DEV3@ `
    @DEV1_ALIAS@ `
    @ATTR_ALIAS@ `
    @FWD_DEV@ `
    @DEV20@ `
	}  *> "$TANGO_TEST_CASE_DIRECTORY/conf_devtest.log"
   

&{@CMAKE_CURRENT_BINARY_DIR@/start_server.ps1 @INST_NAME@ DevTest cpp_test_ds}

&{@CMAKE_CURRENT_BINARY_DIR@/start_server.ps1 @INST_NAME@ FwdTest cpp_test_ds/fwd_ds}
