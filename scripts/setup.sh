#!/bin/bash


usage()
{
    echo "Usage: ${0} [OPTION]..."
    echo "    -h, --help            print this message and exit (other options will be ignored)"
    echo "    -c  --clang           force use of clang compiler (uses default compiler if not set)"
}


ARGS=`getopt -o hc --long help,clang -- "${@}"`

if [ ${?} != 0 ]
then
    echo "Failed parsing options." >&2
    usage
    exit 1
fi

eval set -- "${ARGS}"

help=false
use_clang=false

while true
do
    case "${1}" in
        -h | --help) help=true; shift;;
        -c | --clang) use_clang=true; shift;;
        --) shift; break;;
        *) break ;;
    esac
done

if [ "${help}" == "true" ]
then
    usage
    exit 0
fi

build_dir=`dirname $0`/../build

rm -rf ${build_dir}
mkdir ${build_dir}
cd ${build_dir}

if [ "${use_clang}" == "true" ]
then
    cmake -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++ -DCMAKE_INSTALL_PREFIX=../install ..
else
    cmake -DCMAKE_INSTALL_PREFIX=../install ..
fi

if [ $? -ne 0 ]
then
    exit $?
fi

make -j `nproc` install
if [ $? -ne 0 ]
then
    exit $?
fi
cd ..

install/test/bin/run_all.sh again_quietly

exit 0
