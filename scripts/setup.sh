#!/bin/bash


usage()
{
    echo "Usage: ${0} [OPTION]..."
    echo "    -h, --help            print this message and exit (other options will be ignored)"
    echo "    -c  --clang           force use of clang compiler (uses default compiler if not set)"
    echo "    -b  --build_dir       build directory (use \"build\" by default if not specified)"
    echo "    -i  --install_dir     install directory (use \"install by default if not specified\")"
}


ARGS=`getopt -o hcb:i: --long help,clang,build_dir:,install_dir: -- "${@}"`

if [ ${?} != 0 ]
then
    echo "Failed parsing options." >&2
    usage
    exit 1
fi

eval set -- "${ARGS}"

repo_root="`pwd`/`dirname $0`/.."

help=false
use_clang=false
build_dir=${repo_root}/build
install_dir=${repo_root}/install

while true
do
    case "${1}" in
        -h | --help) help=true; shift;;
        -c | --clang) use_clang=true; shift;;
        -b | --build_dir) build_dir=$2; shift 2;;
        -i | --install_dir) install_dir=$2; shift 2;;
        --) shift; break;;
        *) break ;;
    esac
done

if [ "${help}" == "true" ]
then
    usage
    exit 0
fi

rm -rf ${build_dir}
mkdir ${build_dir}
cd ${build_dir}
rm -rf ${install_dir}
mkdir ${install_dir}

if [ "${use_clang}" == "true" ]
then
    cmake -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++ -DCMAKE_INSTALL_PREFIX=${install_dir} ${repo_root}
else
    cmake -DCMAKE_INSTALL_PREFIX=${install_dir} ${repo_root}
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
