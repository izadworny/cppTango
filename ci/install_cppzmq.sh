#!/usr/bin/env bash

ADDITIONAL_ARGS="-DCMAKE_INSTALL_PREFIX=/home/tango -DCPPZMQ_BUILD_TESTS=OFF"

if [[ -f "$TOOLCHAIN_FILE" && -s "$TOOLCHAIN_FILE" ]]
then
  ADDITIONAL_ARGS="${ADDITIONAL_ARGS} -DCMAKE_TOOLCHAIN_FILE=/home/tango/src/${TOOLCHAIN_FILE}"
fi

docker exec cpp_tango mkdir -p /home/tango/cppzmq/build

echo "Build cppzmq"
docker exec cpp_tango cmake -H/home/tango/cppzmq -B/home/tango/cppzmq/build ${ADDITIONAL_ARGS}
if [ $? -ne "0" ]
then
    exit -1
fi
echo "Install cppzmq"
docker exec cpp_tango make -C /home/tango/cppzmq/build install
if [ $? -ne "0" ]
then
    exit -1
fi
