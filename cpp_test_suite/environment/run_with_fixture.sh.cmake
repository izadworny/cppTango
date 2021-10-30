#!/usr/bin/env bash
# vim: syntax=sh

set -o errexit
set -o nounset

: "${TANGO_TEST_CASE_SKIP_FIXTURE:=}" # Not set by default.

default_valgrind_options=(
    --tool=memcheck
    --leak-check=full
    --track-origins=yes
    --suppressions='@PROJECT_SOURCE_DIR@/suppressions-valgrind.supp'
    --show-error-list=yes
    --verbose
    --error-exitcode=80)

default_valgrind_options_str=$(printf "'%s' " "${default_valgrind_options[@]}")

# Export valgrind options to use from the start_server.sh script.
set -o allexport
: "${TANGO_TEST_CASE_VALGRIND_CLIENT:=}"
: "${TANGO_TEST_CASE_VALGRIND_SERVER:=}"
: "${TANGO_TEST_CASE_VALGRIND_OPTIONS:="${default_valgrind_options_str}"}"
set +o allexport

if [ $# -lt 1 ]; then
    echo "usage: $0 <executable> [args...]"
    exit 1
fi

if ! hash docker 2>/dev/null; then
  echo "Can not run the tests as docker is missing"
  exit 1
fi

# Construct unique name for this test execution. We are using current date with
# nanosecond precision to ensure that subsequent test runs will get a new name.
tc_program="$1"
tc_run_name="$(basename "${tc_program}")_$(date '+%Y%m%d.%H%M%S.%N')"
shift 1

tc_mysql_container="mysql_db_${tc_run_name}"
tc_tango_container="tango_cs_${tc_run_name}"

# This path must be exported as other scripts use it for e.g. storing logs.
export TANGO_TEST_CASE_DIRECTORY="@PROJECT_BINARY_DIR@/test_results/${tc_run_name}"
mkdir -p "${TANGO_TEST_CASE_DIRECTORY}"

# Cleanup function. This is attached to the EXIT signal.
function teardown {
    tc_exit_code="$?"

    # Synchronously stop all running device servers. We need their exit codes.
    "@CMAKE_CURRENT_BINARY_DIR@/kill_server.sh" || true

    # Stop docker containers from a background subshell. We do not need to wait
    # for them. Also close stdout and stderr to allow ctest finish the test early.
    (
        exec 1<&-
        exec 2<&-
        docker stop "${tc_tango_container}" "${tc_mysql_container}" &>/dev/null || true
    ) &

    # Wait up to 5s for device servers to produce files with exit status.
    num_servers="$(wc -l <"${TANGO_TEST_CASE_DIRECTORY}/server_pids")"
    for step in {1..50}; do
        exit_code_files="$(find "${TANGO_TEST_CASE_DIRECTORY}" -name '*_*_*_exit_code.txt')"
        num_exit_code_files="$(wc -l <<<"${exit_code_files}")"
        if [[ "${num_servers}" == "${num_exit_code_files}" ]]; then
            break;
        fi
        sleep 0.1
    done

    failure=""

    if [[ "${num_servers}" != "${num_exit_code_files}" ]]; then
        echo "Timeout waiting for server exit status" \
            | tee -a "${TANGO_TEST_CASE_DIRECTORY}/testcase.log"
        failure="true"
    fi

    if (( $num_exit_code_files >= 1 )); then
        while IFS= read -r file; do
            read -r server_exit_code <"${file}"
            if [[ "${server_exit_code}" != "0" ]]; then
                echo "Non-zero exit status in $(basename "${file}"): ${server_exit_code}" \
                    | tee -a "${TANGO_TEST_CASE_DIRECTORY}/testcase.log"
                failure="true"
            fi
        done <<<"${exit_code_files}"
    fi

    # Overwrite script exit code only if it was successful. This is needed
    # to fail the testcase if device server did not exit cleanly. The original
    # exit code can always be recovered from the testcase_exit_code.txt file.
    if [[ "${failure}" == "true" && "${tc_exit_code}" == "0" ]]; then
        exit 1
    fi
}

if [[ -z "${TANGO_TEST_CASE_SKIP_FIXTURE}" ]]; then
    trap teardown EXIT
    eval $(
        set -e
        source "@CMAKE_CURRENT_BINARY_DIR@/setup_database.sh" \
            "${tc_mysql_container}" \
            "${tc_tango_container}"
        echo "export TANGO_HOST=$TANGO_HOST"
    )
    "@CMAKE_CURRENT_BINARY_DIR@/setup_devices.sh"
fi

set +o errexit
set -o pipefail

if [[ -z "${TANGO_TEST_CASE_VALGRIND_CLIENT}" ]]; then
    "${tc_program}" "$@" 2>&1 | tee "${TANGO_TEST_CASE_DIRECTORY}/testcase.log"
else
    declare -a "valgrind_options=($TANGO_TEST_CASE_VALGRIND_OPTIONS)"
    valgrind "${valgrind_options[@]}" "${tc_program}" "$@" \
        2>&1 | tee "${TANGO_TEST_CASE_DIRECTORY}/testcase.log"
fi

tc_exit_code="$?"

set +o pipefail
set -o errexit

echo "${tc_exit_code}" > "${TANGO_TEST_CASE_DIRECTORY}/testcase_exit_code.txt"
exit "${tc_exit_code}"
