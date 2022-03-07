#!/usr/bin/env bash

set -e

echo "############################"
echo "CMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE"
echo "OS_TYPE=$OS_TYPE"
echo "TANGO_HOST=$TANGO_HOST"
echo "USE_PCH=$USE_PCH"
echo "BUILD_SHARED_LIBS=$BUILD_SHARED_LIBS"
echo "TANGO_USE_USING_NAMESPACE=$TANGO_USE_USING_NAMESPACE"
echo "WARNINGS_AS_ERRORS=$WARNINGS_AS_ERRORS"
echo "############################"

docker exec cpp_tango mkdir -p /home/tango/src/build

# set defaults
MAKEFLAGS=${MAKEFLAGS:- -j $(nproc)}
USE_PCH=${USE_PCH:-OFF}
BUILD_SHARED_LIBS=${BUILD_SHARED_LIBS:-ON}
WARNINGS_AS_ERRORS=${WARNINGS_AS_ERRORS:-OFF}

docker exec cpp_tango cmake                                \
  -H/home/tango/src                                        \
  -B/home/tango/src/build                                  \
  -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}                 \
  -DCMAKE_VERBOSE_MAKEFILE=ON                              \
  -DCPPZMQ_BASE=/home/tango                                \
  -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}                   \
  -DUSE_PCH=${USE_PCH}                                     \
  -DTANGO_USE_USING_NAMESPACE=${TANGO_USE_USING_NAMESPACE} \
  -DTANGO_USE_JPEG=${TANGO_USE_JPEG} \
  -DWARNINGS_AS_ERRORS=${WARNINGS_AS_ERRORS}               \
  -DTANGO_ENABLE_COVERAGE=${TANGO_ENABLE_COVERAGE:-OFF}

docker exec cpp_tango make -C /home/tango/src/build $MAKEFLAGS
