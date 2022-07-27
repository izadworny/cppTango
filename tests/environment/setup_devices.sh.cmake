#!/usr/bin/env bash
# vim: syntax=sh

set -e

export TANGO_TEST_CASE_DIRECTORY="${TANGO_TEST_CASE_DIRECTORY:-.}"

"@PROJECT_BINARY_DIR@/conf_devtest" \
    @SERV_NAME@/@INST_NAME@ \
    @DEV1@ \
    @DEV2@ \
    @DEV3@ \
    @DEV1_ALIAS@ \
    @ATTR_ALIAS@ \
    @FWD_DEV@ \
    @DEV20@ \
    &> "${TANGO_TEST_CASE_DIRECTORY}/conf_devtest.log"

"@CMAKE_CURRENT_BINARY_DIR@/start_server.sh" @INST_NAME@ DevTest device_server/generic

"@CMAKE_CURRENT_BINARY_DIR@/start_server.sh" @INST_NAME@ FwdTest device_server/forward
