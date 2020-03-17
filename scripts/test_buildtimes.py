#!/usr/bin/python3

import os
import shutil
import subprocess
import time
import multiprocessing


repo_root = os.path.dirname(os.path.realpath(__file__)) + '/../'

src_dir = repo_root + 'test_buildtimes/src'
prep_dir = repo_root + 'test_buildtimes/prepare'

# remove and recreate source directory
shutil.rmtree(src_dir, ignore_errors=True)
os.makedirs(src_dir)

# copy matchable to source directory
matchable_source = repo_root + 'src/matchable.h'
shutil.copy(matchable_source, src_dir)

# remove and recreate CMakeLists.txt
try:
    os.remove(prep_dir + '/../CMakeLists.txt')
except OSError:
    pass
shutil.copy(prep_dir + '/CMakeLists.txt', prep_dir + '/..')

# copy TimeUnit.h
shutil.copy(prep_dir + '/TimeUnit.h', src_dir)

# copy IncludedAllOverThePlace
shutil.copy(prep_dir + '/full/IncludedAllOverThePlace.h', src_dir)
shutil.copy(prep_dir + '/full/IncludedAllOverThePlace.cpp', src_dir)

# create main.cpp and LikesIncludedAllOverThePlace_i.h/cpp for i in [0..107]
main_content_start = '#include "TimeUnit.h"\n'
main_content_main = ''

for i in range(0, 108):
    likes_h_content  = '#pragma once\n'
    likes_h_content += '#include "IncludedAllOverThePlace.h"\n'
    likes_h_content += 'class LikesIncludedAllOverThePlace_' + str(i)
    likes_h_content += ' : public IncludedAllOverThePlace\n'
    likes_h_content += '{ public: explicit LikesIncludedAllOverThePlace_' + str(i)
    likes_h_content += '(TimeUnit::Type tu); };\n\n'

    likes_h_file = open(src_dir + '/LikesIncludedAllOverThePlace_' + str(i) + '.h', 'w')
    likes_h_file.write(likes_h_content)
    likes_h_file.close()

    likes_cpp_content  = '#include "LikesIncludedAllOverThePlace_' + str(i) + '.h"\n'
    likes_cpp_content += 'LikesIncludedAllOverThePlace_' + str(i)
    likes_cpp_content += '::LikesIncludedAllOverThePlace_' + str(i) + '(TimeUnit::Type tu)\n'
    likes_cpp_content += '    : IncludedAllOverThePlace(tu)\n'
    likes_cpp_content += '{ for (auto t : TimeUnit::variants()) if (t == tu) (void) t; }\n\n'

    likes_cpp_file = open(src_dir + '/LikesIncludedAllOverThePlace_' + str(i) + '.cpp', 'w')
    likes_cpp_file.write(likes_cpp_content)
    likes_cpp_file.close()

    main_content_start += '#include "LikesIncludedAllOverThePlace_' + str(i) + '.h"\n'
    main_content_main += '    LikesIncludedAllOverThePlace_' + str(i) + ' l_' + str(i)
    main_content_main += '{TimeUnit::Seconds::grab()};\n'

main_content = main_content_start
main_content += 'int main()\n{    ' + main_content_main
main_content += '}\n\n'

main_file = open(src_dir + '/main.cpp', 'w')
main_file.write(main_content)
main_file.close()


# remove and recreate build directory
shutil.rmtree(src_dir + '/../build', ignore_errors=True)
os.makedirs(src_dir + '/../build')
os.chdir(src_dir + '/../build')

# run cmake
if subprocess.run(['cmake', '..']).returncode != 0:
    print('cmake failed')
    exit(-1)

# run make tracking how much time it needs
start = time.time()
subprocess.run(['make', '-j' + str(multiprocessing.cpu_count())])
end = time.time()

print('\nmake finished in ' + str(end - start) + ' seconds')

os.remove(prep_dir + '/../CMakeLists.txt')
shutil.rmtree(src_dir + '/../build')
shutil.rmtree(src_dir)
