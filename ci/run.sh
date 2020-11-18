#!/usr/bin/env bash

set -e

echo "############################"
echo "CMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE"
echo "OS_TYPE=$OS_TYPE"
echo "TANGO_HOST=$TANGO_HOST"
echo "USE_PCH=$USE_PCH"
echo "BUILD_SHARED_LIBS=$BUILD_SHARED_LIBS"
echo "TANGO_USE_USING_NAMESPACE=$TANGO_USE_USING_NAMESPACE"
echo "TANGO_USE_LIBCPP=$TANGO_USE_LIBCPP"
echo "WARNINGS_AS_ERRORS=$WARNINGS_AS_ERRORS"
echo "############################"

docker exec cpp_tango mkdir -p /home/tango/src/build

# set defaults
MAKEFLAGS=${MAKEFLAGS:- -j $(nproc)}
TANGO_USE_LIBCPP=${TANGO_USE_LIBCPP:-OFF}
USE_PCH=${USE_PCH:-OFF}
BUILD_SHARED_LIBS=${BUILD_SHARED_LIBS:-ON}
WARNINGS_AS_ERRORS=${WARNINGS_AS_ERRORS:-OFF}
SOURCE_DIR=/home/tango/src
BUILD_DIR=${SOURCE_DIR}/build

ADDITIONAL_ARGS=""

docker exec cpp_tango cmake                                \
  -H${SOURCE_DIR}                                          \
  -B${BUILD_DIR}                                           \
  -DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}                 \
  -DCMAKE_VERBOSE_MAKEFILE=ON                              \
  -DCPPZMQ_BASE=/home/tango                                \
  -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}                   \
  -DUSE_PCH=${USE_PCH}                                     \
  -DTANGO_USE_USING_NAMESPACE=${TANGO_USE_USING_NAMESPACE} \
  -DTANGO_USE_JPEG=${TANGO_USE_JPEG}                        \
  -DTANGO_USE_LIBCPP=${TANGO_USE_LIBCPP}                     \
  -DWARNINGS_AS_ERRORS=${WARNINGS_AS_ERRORS}               \
  -DTANGO_ENABLE_COVERAGE=${TANGO_ENABLE_COVERAGE:-OFF}    \
  ${ADDITIONAL_ARGS}

docker exec cpp_tango make -C ${BUILD_DIR} $MAKEFLAGS
