#!/usr/bin/env bash

LLVM_VERSION=14
VERSION=0

docker_image="cpptango-clang-format-${LLVM_VERSION}-${VERSION}"
top_level=$(git rev-parse --show-toplevel)

if hash clang-format 2>/dev/null; then
  # format: Debian clang-format version 11.0.1-2
  version_default=$(clang-format --version 2>/dev/null | grep -oP " [[:digit:]]+.[[:digit:]]+")
  if [[ "$version_default" == " ${LLVM_VERSION}.0" ]]; then
    command="clang-format"
  fi
fi

if [[ -z "$command" ]]; then
  if hash clang-format-${LLVM_VERSION} 2>/dev/null; then
    version_default=$(clang-format-${LLVM_VERSION} --version 2>/dev/null | grep -oP " [[:digit:]]+.[[:digit:]]+")
    if [[ "$version_default" == " ${LLVM_VERSION}.0" ]]; then
      command="clang-format-${LLVM_VERSION}"
    fi
  fi
fi

if [[ -z "$command" ]]; then
  if ! hash docker 2>/dev/null; then
    echo "Could not format the code as docker is missing and we could not find a suitable clang-format with version ${LLVM_VERSION}."
    exit 1
  fi

  echo "Using the slow docker fallback; Install clang-format-${LLVM_VERSION} to avoid that."

  docker build --build-arg=LLVM_VERSION="${LLVM_VERSION}" ${top_level}/ci/clang-format -t ${docker_image} > /dev/null
  if [ $? -ne 0 ]; then
    echo "Could not build docker image"
    exit 1
  fi

  command="docker run --user root -v ${top_level}:/home/tango ${docker_image} clang-format-${LLVM_VERSION}"
fi

cd $top_level
files=$(git ls-files '*.h' '*.cpp' '*.tpp' ':^*/cxxtest/*')
$command -i $files
