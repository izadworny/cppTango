#!/usr/bin/env bash
# vim: syntax=sh

exec docker run --rm \
    --read-only \
    --network=host \
    -e TANGO_HOST="$TANGO_HOST" \
    --entrypoint /usr/local/bin/tango_admin \
    tangocs/tango-cs:latest "$@"
