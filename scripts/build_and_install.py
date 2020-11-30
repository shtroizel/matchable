#!/usr/bin/env python3

import os
import multiprocessing
import shutil
import subprocess
import sys
import getopt

from shutil import which


def usage():
    print('options for building matchable:\n')
    print('    -h, --help                   print this message\n')
    print('    -c  --clang                  force use of clang compiler')
    print('                                   * system compiler used by default\n')
    print('    -b  --build_dir              build directory')
    print('                                   * defaults to <matchable root>/build')
    print('                                   * relative paths are relative to <matchable root>\n')
    print('    -i  --install_dir            install directory')
    print('                                   * defaults to <matchable root>/install')
    print('                                   * relative paths are relative to build_dir\n')
    print('    -j  --jobs                   max jobs')
    print('                                   * default is cpu count ['                                    \
            + str(multiprocessing.cpu_count()) + ']\n')
    print('    -l  --lib_only               build library only')
    print('                                   * by default tests are built too\n')
    print('    -t  --test                   if build succeeds also run tests')
    print('                                   * incompatible with \'-l\'\n')
    print('    -d  --debug                  debug build')
    print('                                   * use -DCMAKE_BUILD_TYPE=Debug (default is: '                \
            + '-DCMAKE_BUILD_TYPE=Release)\n')
    print('    -a  --alphabetical_only      variants \'by string\' only (omit \'by index\')')
    print('                                   * adds definition MATCHABLE_OMIT_BY_INDEX which will:')
    print('                                     - omit variants_by_index()')
    print('                                     - omit as_index()')
    print('                                     - omit from_index()')
    print('                                     - omit lt_by_index()\n')



def build_and_install(use_clang, build_dir, install_dir, jobs, lib_only, run_tests, debug,
                      alphabetical_only):
    start_dir = os.getcwd()

    matchable_root = os.path.dirname(os.path.realpath(__file__)) + '/../'
    os.chdir(matchable_root)

    if build_dir == '':
        build_dir = matchable_root + '/build'
    while build_dir[-1] == '/':
        build_dir = build_dir[:-1]

    if install_dir == '':
        install_dir = matchable_root + '/install'
    while install_dir[-1] == '/':
        install_dir = install_dir[:-1]

    shutil.rmtree(build_dir, ignore_errors=True)
    os.makedirs(build_dir)
    os.chdir(build_dir)

    cmake_cmd = ['cmake', '-DCMAKE_INSTALL_PREFIX=' + install_dir]

    if lib_only:
        cmake_cmd.append('-DLIB_ONLY=ON')

    if use_clang:
        clang_c = which('clang')
        clang_cxx = which('clang++')
        if clang_c is None or clang_cxx is None:
            print('search for clang compiler failed')
            exit(1)
        cmake_cmd.append('-DCMAKE_C_COMPILER=' + clang_c)
        cmake_cmd.append('-DCMAKE_CXX_COMPILER=' + clang_cxx)

    if debug:
        cmake_cmd.append('-DCMAKE_BUILD_TYPE=Debug')
    else:
        cmake_cmd.append('-DCMAKE_BUILD_TYPE=Release')

    if alphabetical_only:
        cmake_cmd.append('-DOMIT_BY_INDEX=ON')

    cmake_cmd.append(matchable_root)

    if subprocess.run(cmake_cmd).returncode != 0:
        print('cmake failed')
        os.chdir(matchable_root)
        exit(1)

    if subprocess.run(['make', '-j' + jobs]).returncode != 0:
        print('make failed')
        os.chdir(matchable_root)
        exit(1)

    if subprocess.run(['make', '-j' + jobs, 'install']).returncode != 0:
        print('make install failed, trying sudo make install...')
        if subprocess.run(['sudo', 'make', '-j' + jobs, 'install']).returncode != 0:
            print('sudo make install failed')
            os.chdir(matchable_root)
            exit(1)


    if not lib_only and run_tests:
        if subprocess.run([install_dir + '/share/matchable/test/bin/run_all.sh',
                           'again_quietly']).returncode != 0:
            print('run_all.sh failed')
            os.chdir(matchable_root)
            exit(1)

    os.chdir(start_dir)


def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], 'hcb:i:j:ltda',
                ['help', 'clang', 'build', 'install', 'jobs', 'lib_only', 'test', 'debug',
                 'alphabetical_only'])
    except getopt.GetoptError as err:
        print(err)
        usage()
        sys.exit(2)


    use_clang = False
    build_dir = ''
    install_dir = ''
    jobs = str(multiprocessing.cpu_count())
    lib_only = False
    run_tests = False
    debug = False
    alphabetical_only = False

    for o, a in opts:
        if o in ('-h', '--help'):
            usage()
            sys.exit()
        elif o in ('-c', '--clang'):
            use_clang = True
        elif o in ('-b', '--build_dir'):
            build_dir = a
        elif o in ('-i', '--install_dir'):
            install_dir = a
        elif o in ('-j', '--jobs'):
            jobs = a
        elif o in ('-l', '--lib_only'):
            lib_only = True
        elif o in ('-t', '--test'):
            run_tests = True
        elif o in ('-d', '--debug'):
            debug = True
        elif o in ('-a', '--alphabetical_only'):
            alphabetical_only = True
        else:
            assert False, "unhandled option"

    build_and_install(use_clang, build_dir, install_dir, jobs, lib_only, run_tests, debug,
                      alphabetical_only)

    exit(0)


if __name__ == "__main__":
    main()
