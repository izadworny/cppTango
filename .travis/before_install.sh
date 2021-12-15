#!/usr/bin/env bash

set -e

if [[ "$RUN_TESTS" == "ON" ]]
then
  docker pull registry.gitlab.com/tango-controls/docker/mysql:5.16-mysql-5
  docker pull registry.gitlab.com/tango-controls/docker/tango-db:5.16
fi

if [[ "$COVERALLS" == "ON" ]]
then
  git clone --depth 1 https://github.com/JoakimSoderberg/coveralls-cmake.git
fi

if [[ "$STOCK_CPPZMQ" == "OFF" ]]
then
  git clone -b v4.7.1 --depth 1 https://github.com/zeromq/cppzmq.git
else
  mkdir cppzmq
fi

git clone --depth 1 https://gitlab.com/tango-controls/tango-idl.git idl
