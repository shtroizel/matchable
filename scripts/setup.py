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



def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], 'hcb:i:', ['help', 'clang', 'build', 'install'])
    except getopt.GetoptError as err:
        print(err)
        usage()
        sys.exit(2)

    use_clang = False
    build_dir = repo_root + 'build'
    install_dir = repo_root + 'install'

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
        else:
            assert False, "unhandled option"

    shutil.rmtree(build_dir, ignore_errors=True)
    os.makedirs(build_dir)
    shutil.rmtree(install_dir, ignore_errors=True)
    os.makedirs(install_dir)

    os.chdir(build_dir)

    if use_clang:
        cmake_cmd = ['cmake', '-DCMAKE_C_COMPILER=/usr/bin/clang', '-DCMAKE_CXX_COMPILER=/usr/bin/clang++',
                     '-DCMAKE_INSTALL_PREFIX=' + install_dir, repo_root]
    else:
        cmake_cmd = ['cmake', '-DCMAKE_INSTALL_PREFIX=' + install_dir, repo_root]

    if subprocess.run(cmake_cmd).returncode != 0:
        print('cmake failed')
        os.chdir(repo_root)
        exit(1)

    if subprocess.run(['make', '-j' + str(multiprocessing.cpu_count()), 'install']).returncode != 0:
        print('make failed')
        os.chdir(repo_root)
        exit(1)

    if subprocess.run([install_dir + '/share/matchable/test/bin/run_all.sh', 'again_quietly']).returncode != 0:
        print('run_all.sh failed')
        os.chdir(repo_root)
        exit(1)

    os.chdir(repo_root)
    exit(0)



if __name__ == "__main__":
    main()
