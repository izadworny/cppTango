#!/usr/bin/env bash
# vim: syntax=sh

export SHELLOPTS # Make below options effective also in subshells.

set -o errexit
set -o nounset

mkdir -p "${TANGO_TEST_CASE_DIRECTORY}" # Must be set by the calling process.

instance="$1"
server="${2:-DevTest}"
server_path="${3:-cpp_test_ds}"

# A subshell that will run device server process and collect the exit code.
(
    # A subshell that will exec into device server after redirecting stdout.
    # Exec is needed for signal handling (e.g. for sending TERM on shutdown),
    # otherwise we would need to trap the signals and forward them manually.
    (
        server_pid="${BASHPID}"
        echo "Starting ${server}/${instance} (PID: ${server_pid})"

        # Store PID in a file so that the process can be shut down later.
        echo "$server_pid" >> "${TANGO_TEST_CASE_DIRECTORY}/server_pids"

        # Close and reopen stdout and stderr. This is needed to avoid blocking
        # 'tee' command if this script is run as a subprocess of the testcase.
        exec 1<&-
        exec 2<&-
        exec 1<>"${TANGO_TEST_CASE_DIRECTORY}/${server}_${instance}_${server_pid}_stdout.log"
        exec 2>&1

        exec "@PROJECT_BINARY_DIR@/${server_path}/${server}" "$instance" -v5
    ) &
    server_pid="$!"

    # Close the descriptors (same reason as in the subshell above).
    exec 1<&-
    exec 2<&-

    # Unset the errexit flag. Wait will fail if the waited process failed.
    set +o errexit
    wait "$server_pid"
    server_exit_code="$?"

    echo "$server_exit_code" > "${TANGO_TEST_CASE_DIRECTORY}/${server}_${instance}_${server_pid}_exit_code.txt"
    exit "$server_exit_code"
) &

if hash tango_admin 2>/dev/null; then
    instance_lower="$(echo "$server/$instance" | awk '{print tolower($0)}')"
    tango_admin --ping-device "dserver/${instance_lower}" 7
else
    sleep 7
fi
