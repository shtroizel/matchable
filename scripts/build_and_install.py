#!/usr/bin/env python3

import os
import multiprocessing
import shutil
import subprocess
import sys
import getopt



repo_root = os.path.dirname(os.path.realpath(__file__)) + '/../'



def usage():
    print(sys.argv[0] + ' [OPTION]')
    print('    -h, --help            print this message')
    print('    -c  --clang           force use of clang compiler (uses default compiler if not set)')
    print('    -b  --build_dir       build directory (defaults to <repo root>/build)')
    print('    -i  --install_dir     install directory (defaults to <repo root>/install)')
    print('                          note that relative paths are relative to build_dir')
    default_cpu_count = str(multiprocessing.cpu_count())
    print('    -j  --jobs            max jobs (default is cpu count [' + default_cpu_count + '])')
    print('    -t  --test            if build succeeds also run tests')



def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], 'hcb:i:j:t', ['help', 'clang', 'build', 'install',
                                                               'jobs', 'test'])
    except getopt.GetoptError as err:
        print(err)
        usage()
        sys.exit(2)

    use_clang = False
    build_dir = repo_root + 'build'
    install_dir = repo_root + 'install'
    jobs = str(multiprocessing.cpu_count())
    run_tests = False

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
        elif o in ('-t', '--test'):
            run_tests = True
        else:
            assert False, "unhandled option"

    shutil.rmtree(build_dir, ignore_errors=True)
    os.makedirs(build_dir)
    shutil.rmtree(install_dir, ignore_errors=True)
    os.makedirs(install_dir)

    os.chdir(build_dir)

    cmake_cmd = ['cmake', '-DCMAKE_INSTALL_PREFIX=' + install_dir]

    if use_clang:
        cmake_cmd.append('-DCMAKE_C_COMPILER=/usr/bin/clang')
        cmake_cmd.append('-DCMAKE_CXX_COMPILER=/usr/bin/clang++')

    cmake_cmd.append(repo_root)

    if subprocess.run(cmake_cmd).returncode != 0:
        print('cmake failed')
        os.chdir(repo_root)
        exit(1)

    if subprocess.run(['make', '-j' + jobs, 'install']).returncode != 0:
        print('make failed')
        os.chdir(repo_root)
        exit(1)

    if run_tests:
        if subprocess.run([install_dir + '/share/matchable/test/bin/run_all.sh',
                           'again_quietly']).returncode != 0:
            print('run_all.sh failed')
            os.chdir(repo_root)
            exit(1)

    os.chdir(repo_root)
    exit(0)



if __name__ == "__main__":
    main()
