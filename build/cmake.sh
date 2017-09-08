#!/bin/bash -x

OUT_DIR=linux
mkdir -p ${OUT_DIR}
pushd ${OUT_DIR}

function exec_cmake() {
    mkdir -p $1
    pushd $1
    cmake -DCMAKE_BUILD_TYPE=$1 ../../..
    popd
}

exec_cmake Debug
exec_cmake Release

popd
