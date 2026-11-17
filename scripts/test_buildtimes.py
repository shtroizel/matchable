#!/usr/bin/env python3

import csv
import os
import shutil
import subprocess
import time
import multiprocessing
import random
import sys
import getopt

from shutil import which


repo_root = os.path.dirname(os.path.realpath(__file__)) + '/../'
work_dir = repo_root + 'test_buildtimes'
src_dir = work_dir + '/src'
build_dir = work_dir + '/build'
prep_dir = work_dir + '/prepare'

# (name, translation units, variants per type): one type per translation unit
scale_cases = [
    ('small types', 300, 10),
    ('leaf-sized types', 60, 150),
    ('one big type', 1, 1000),
]

cxx_keywords = {
    'alignas', 'alignof', 'and', 'asm', 'auto', 'bitand', 'bitor', 'bool', 'break', 'case', 'catch', 'char',
    'class', 'compl', 'concept', 'const', 'consteval', 'constexpr', 'constinit', 'continue', 'decltype',
    'default', 'delete', 'do', 'double', 'else', 'enum', 'explicit', 'export', 'extern', 'false', 'float',
    'for', 'friend', 'goto', 'if', 'inline', 'int', 'long', 'mutable', 'namespace', 'new', 'noexcept',
    'not', 'nullptr', 'operator', 'or', 'private', 'protected', 'public', 'register', 'requires', 'return',
    'short', 'signed', 'sizeof', 'static', 'struct', 'switch', 'template', 'this', 'throw', 'true', 'try',
    'typedef', 'typeid', 'typename', 'union', 'unsigned', 'using', 'virtual', 'void', 'volatile', 'while',
    'xor',
}


def usage():
    print(sys.argv[0] + ' [OPTION]...')
    print('    -h, --help            print this message')
    print('    -c  --clang           force use of clang compiler (uses default compiler if not set)')
    print('    -d  --debug           build with debug symbols')
    print('    -m  --minimal         build with minimal features')
    print('                            * use alphabetical only (omit by index)')
    print('    -j  --jobs            max jobs')
    print('                            * default is cpu count ['
          + str(multiprocessing.cpu_count()) + ']')
    print('    -r  --repeat          builds per case, the fastest is reported (default 3)')
    print('                            * with -b, current and baseline builds alternate')
    print('    -s  --scale           time how builds grow with types and variants instead of comparing')
    print('                          full definitions with forward declarations:')
    for name, tus, variants in scale_cases:
        print('                            * ' + name + ': ' + str(tus)
              + ' translation unit(s), one type of ' + str(variants) + ' variants each')
    print('        --sweep           comma separated variants per type, e.g. 10,50,100,250,500,1000')
    print('                            * one build per value of jobs (-j) translation units, each one type')
    print('                              of that many variants; each compile of 1000 variants needs about')
    print('                              2 GB of memory')
    print('                            * each translation unit compile is timed on its own (main.cpp, link')
    print('                              and cmake excluded) and the mean goes to a csv file for')
    print('                              scripts/plot_sweep.py')
    print('                            * on cpus with fast and slow cores, -j above the fast core count')
    print('                              puts compiles on slow cores: compare runs at the same -j')
    print('    -o  --output          csv file for --sweep')
    print('                            * default <matchable root>/build_sweep_buildtimes.csv')
    print('    -b  --baseline        include directory holding another matchable/matchable.h')
    print('                            * every case is also built with it and the times compared')
    print('                            * builds alternate, taking turns going first')
    print('                            * for example the include directory of a checkout of another commit')
    print('\ncompiler CPU time (user + system) is reported first and ratios use it: it depends less on')
    print('scheduling and -j than wall time does. Times are only comparable with times from the same')
    print('machine and load, so compare within one run (ratios), not with numbers from elsewhere')


def variant_names(count, rng):
    names = set()
    while len(names) < count:
        name = ''.join(rng.choice('abcdefghijklmnopqrstuvwxyz') for _ in range(rng.randint(3, 12)))
        if name not in cxx_keywords:
            names.add(name)
    return sorted(names)


def matchable_source(type_name, names):
    lines = ['#include "matchable/matchable.h"', '']
    for start in range(0, len(names), 108):
        macro = 'MATCHABLE' if start == 0 else 'GROW_MATCHABLE'
        lines.append(macro + '(')
        lines.append('    ' + type_name)
        for name in names[start:start + 108]:
            lines.append('    , ' + name)
        lines.append(')')
    lines.append('')
    lines.append('int lookup_' + type_name + '(std::string const & s)')
    lines.append('{')
    lines.append('    bool found = false;')
    lines.append('    return ' + type_name + '::variants_by_string_index_of(s, &found);')
    lines.append('}')
    return '\n'.join(lines) + '\n'


def write_file(path, content):
    with open(path, 'w') as f:
        f.write(content)


def prepare_sources(include_dir):
    shutil.rmtree(src_dir, ignore_errors=True)
    os.makedirs(src_dir + '/matchable')
    shutil.copy(include_dir + '/matchable/matchable.h', src_dir + '/matchable')
    shutil.copy(include_dir + '/matchable/matchable_fwd.h', src_dir + '/matchable')
    shutil.copy(prep_dir + '/CMakeLists.txt', work_dir)


def generate_fwd_full(mode, include_dir):
    prepare_sources(include_dir)
    shutil.copy(prep_dir + '/matchables.h', src_dir)
    shutil.copy(prep_dir + '/' + mode + '/IncludedAllOverThePlace.h', src_dir)
    shutil.copy(prep_dir + '/IncludedAllOverThePlace.cpp', src_dir)

    main_content_start = '#include "matchables.h"\n'
    main_content_main = ''
    for i in range(0, 108):
        name = 'LikesIncludedAllOverThePlace_' + str(i)
        write_file(src_dir + '/' + name + '.h',
                   '#pragma once\n#include "IncludedAllOverThePlace.h"\nclass ' + name
                   + ' : public IncludedAllOverThePlace\n{ public: void foo(int); };\n\n')
        write_file(src_dir + '/' + name + '.cpp',
                   '#include "' + name + '.h"\nvoid ' + name + '::foo(int) {}\n\n')
        main_content_start += '#include "' + name + '.h"\n'
        main_content_main += '    ' + name + ' l_' + str(i) + ';\n'
        main_content_main += '    (void) l_' + str(i) + ';\n'
    write_file(src_dir + '/main.cpp',
               main_content_start + 'int main()\n{\n    ' + main_content_main + '}\n\n')


def generate_scale(tus, variants, include_dir):
    prepare_sources(include_dir)
    rng = random.Random(20261117)
    main_content = '#include <string>\n\n'
    main_calls = ''
    for i in range(tus):
        type_name = 'T' + str(i)
        names = variant_names(variants, rng)
        write_file(src_dir + '/' + type_name + '.cpp', matchable_source(type_name, names))
        main_content += 'int lookup_' + type_name + '(std::string const &);\n'
        main_calls += '    sum += lookup_' + type_name + '(s);\n'
    main_content += '\nint main(int argc, char ** argv)\n{\n'
    main_content += '    std::string const s = argc > 1 ? argv[1] : "";\n    int sum = 0;\n'
    main_content += main_calls + '    return sum == -1 ? 1 : 0;\n}\n'
    write_file(src_dir + '/main.cpp', main_content)


launcher_content = '''import os, resource, subprocess, sys
code = subprocess.run(sys.argv[2:]).returncode
usage = resource.getrusage(resource.RUSAGE_CHILDREN)
source = next((a for a in sys.argv[2:] if a.endswith('.cpp')), '?')
with open(sys.argv[1], 'a') as f:
    f.write(os.path.basename(source) + ' ' + str(usage.ru_utime + usage.ru_stime) + '\\n')
sys.exit(code)
'''


def timed_build(use_clang, debug, minimal, jobs, per_file=None):
    # per_file: a dict filled with cpu seconds per compiled source file
    shutil.rmtree(build_dir, ignore_errors=True)
    os.makedirs(build_dir)
    os.chdir(build_dir)

    cmake_cmd = ['cmake']
    if per_file is not None:
        write_file(build_dir + '/launcher.py', launcher_content)
        launcher = [sys.executable, build_dir + '/launcher.py', build_dir + '/compile_times.txt']
        cmake_cmd.append('-DCMAKE_CXX_COMPILER_LAUNCHER=' + ';'.join(launcher))

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

    if minimal:
        cmake_cmd.append('-DOMIT_BY_INDEX=ON')
    else:
        cmake_cmd.append('-DOMIT_BY_INDEX=OFF')

    cmake_cmd.append('..')

    if subprocess.run(cmake_cmd, stdout=subprocess.DEVNULL).returncode != 0:
        print('cmake failed')
        exit(1)

    # time only the build, not cmake
    cpu_start = cpu_seconds()
    start = time.time()
    return_code = subprocess.run(['cmake', '--build', '.', '--parallel', str(jobs)],
                                 stdout=subprocess.DEVNULL).returncode
    end = time.time()
    cpu = cpu_seconds() - cpu_start
    if per_file is not None and return_code == 0:
        with open(build_dir + '/compile_times.txt') as f:
            for line in f:
                name, seconds = line.split()
                per_file[name] = float(seconds)

    os.chdir(repo_root)
    shutil.rmtree(build_dir)

    if return_code != 0:
        print('build failed')
        exit(1)

    return cpu, end - start


def cpu_seconds():
    import resource
    usage = resource.getrusage(resource.RUSAGE_CHILDREN)
    return usage.ru_utime + usage.ru_stime


def start_csv(path):
    with open(path, 'w', newline='') as f:
        csv.writer(f).writerow(['kind', 'variants', 'types', 'case', 'side', 'value'])


def append_csv(path, rows):
    with open(path, 'a', newline='') as f:
        csv.writer(f).writerows(rows)


def parse_sweep(text):
    try:
        values = [int(v) for v in text.split(',')]
    except ValueError:
        values = []
    if not values or min(values) < 1:
        print('--sweep needs a comma separated list of positive whole numbers, got ' + text)
        sys.exit(2)
    return values


def clean_up():
    shutil.rmtree(src_dir, ignore_errors=True)
    shutil.rmtree(build_dir, ignore_errors=True)
    try:
        os.remove(work_dir + '/CMakeLists.txt')
    except OSError:
        pass


def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], 'hcdmj:r:sb:o:',
                ['help', 'clang', 'debug', 'minimal', 'jobs=', 'repeat=', 'scale', 'baseline=', 'sweep=',
                 'output='])
    except getopt.GetoptError as err:
        print(err)
        usage()
        sys.exit(2)

    use_clang = False
    debug = False
    minimal = False
    jobs = multiprocessing.cpu_count()
    repeat = 3
    scale = False
    baseline = None
    sweep = None
    output = None
    for o, a in opts:
        if o in ('-h', '--help'):
            usage()
            sys.exit()
        elif o in ('-c', '--clang'):
            use_clang = True
        elif o in ('-d', '--debug'):
            debug = True
        elif o in ('-m', '--minimal'):
            minimal = True
        elif o in ('-j', '--jobs'):
            jobs = int(a)
        elif o in ('-r', '--repeat'):
            repeat = int(a)
            if repeat < 1:
                print('--repeat needs at least 1')
                sys.exit(2)
        elif o in ('-s', '--scale'):
            scale = True
        elif o in ('-b', '--baseline'):
            baseline = os.path.abspath(a)
            for header in ('matchable.h', 'matchable_fwd.h'):
                if not os.path.isfile(baseline + '/matchable/' + header):
                    print('no matchable/' + header + ' in ' + baseline)
                    sys.exit(2)
        elif o == '--sweep':
            sweep = parse_sweep(a)
        elif o in ('-o', '--output'):
            output = os.path.abspath(a)
        else:
            assert False, "unhandled option"

    if output is not None and sweep is None:
        print('--output is only used with --sweep')
        sys.exit(2)
    if sweep is not None and scale:
        print('--sweep and --scale are separate modes, choose one')
        sys.exit(2)

    if os.name == 'nt':
        print('compiler cpu time is measured with the resource module, which needs a unix-like system')
        sys.exit(2)

    current = os.path.abspath(repo_root + 'include')
    if sweep is not None:
        cases = [(str(variants) + ' variants (' + str(jobs) + ' x ' + str(variants) + ')',
                  lambda d, variants=variants: generate_scale(jobs, variants, d))
                 for variants in sweep]
    elif scale:
        cases = [(name + ' (' + str(tus) + ' x ' + str(variants) + ')',
                  lambda d, tus=tus, variants=variants: generate_scale(tus, variants, d))
                 for name, tus, variants in scale_cases]
    else:
        cases = [('using full definitions', lambda d: generate_fwd_full('full', d)),
                 ('using forward declarations', lambda d: generate_fwd_full('fwd', d))]

    sides = [('current', current)]
    if baseline is not None:
        sides.append(('baseline', baseline))

    # each sweep point is written as soon as it is measured, so stopping early keeps the finished points
    path = output or repo_root + 'build_sweep_buildtimes.csv'
    if sweep is not None:
        start_csv(path)

    # times[case][side] = [(cpu, wall), ...], one per repeat; per_tu[case][side] = [mean cpu, ...]
    times = []
    per_tu = []
    try:
        for i, (name, generate) in enumerate(cases):
            times.append({side: [] for side, _ in sides})
            per_tu.append({side: [] for side, _ in sides})
            for r in range(repeat):
                for side, include_dir in (sides if r % 2 == 0 else sides[::-1]):
                    print('building: ' + name + ', ' + side + ' ' + str(r + 1) + '/' + str(repeat),
                          flush=True)
                    generate(include_dir)
                    per_file = {} if sweep is not None else None
                    times[-1][side].append(timed_build(use_clang, debug, minimal, jobs, per_file))
                    if per_file is not None:
                        tus = [cpu for f, cpu in per_file.items() if f != 'main.cpp']
                        per_tu[-1][side].append(sum(tus) / len(tus))
            if sweep is not None:
                append_csv(path, [['build', sweep[i], jobs, 'cpu seconds per translation unit', side,
                                   '{:.3f}'.format(min(per_tu[-1][side]))]
                                  for side, _ in sides])
    finally:
        clean_up()

    print('\n\n******* Build Time Performance Summary *******')
    print('seconds, fastest of ' + str(repeat) + ' build(s) each, -j ' + str(jobs))
    print('slowest cpu in parentheses: far above the fastest means the machine was busy or throttled\n')
    width = max(len(name) for name, _ in cases)
    for (name, _), case_times in zip(cases, times):
        line = '  ' + name.rjust(width) + ':'
        for side, _ in sides:
            cpus = [cpu for cpu, _ in case_times[side]]
            wall = min(wall for _, wall in case_times[side])
            line += '   ' + side + ' cpu ' + '{:8.2f}'.format(min(cpus)) + ' (' + '{:.2f}'.format(max(cpus))
            line += ')  wall ' + '{:7.2f}'.format(wall)
        if baseline is not None:
            current_cpu = min(cpu for cpu, _ in case_times['current'])
            baseline_cpu = min(cpu for cpu, _ in case_times['baseline'])
            line += '   cpu ratio ' + '{:5.2f}'.format(current_cpu / baseline_cpu)
        print(line)
    if sweep is not None:
        print('\nwrote ' + path)
    elif not scale:
        full, fwd = [min(cpu for cpu, _ in case_times['current']) for case_times in times]
        print('\n  full / forward cpu: ' + '{:.2f}'.format(full / fwd))


if __name__ == "__main__":
    main()
