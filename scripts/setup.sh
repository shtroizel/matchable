#!/bin/bash

build_dir=`dirname $0`/../build

rm -rf ${build_dir}
mkdir ${build_dir}
cd ${build_dir}

if [ "${1}" == "clang" ]
then
    cmake -DCMAKE_CXX_COMPILER="/usr/bin/clang++" -DCMAKE_INSTALL_PREFIX=../install ..
else
    cmake -DCMAKE_INSTALL_PREFIX=../install ..
fi

if [ $? -ne 0 ]
then
    exit $?
fi
make -j install
if [ $? -ne 0 ]
then
    exit $?
fi
cd ..
install/test/bin/run_all.sh again_quietly

exit 0
