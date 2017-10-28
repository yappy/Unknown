#!/bin/bash -x

BUILD_DIR=build
mkdir -p ${BUILD_DIR}
pushd ${BUILD_DIR}

function exec_cmake() {
    mkdir -p $1
    pushd $1
    cmake -DCMAKE_BUILD_TYPE=$1 ../../../..
    popd
}

exec_cmake Debug
exec_cmake Release
exec_cmake RelWithDebInfo
exec_cmake MinSizeRel

popd
