#!/usr/bin/env python3

import os
import shutil
import subprocess
import multiprocessing
import random
import sys
import getopt

from shutil import which

sys.dont_write_bytecode = True
from test_buildtimes import append_csv, matchable_source, parse_sweep, start_csv, variant_names


repo_root = os.path.dirname(os.path.realpath(__file__)) + '/../'

cmake_content = '''cmake_minimum_required(VERSION 3.16.3)
project(matchable_bench_lookup)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra")
include_directories(${MATCHABLE_INCLUDE})
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_executable(bench_lookup ${SRC_FILES})
'''

main_content = r'''#include <algorithm>
#include <chrono>
#include <cstdio>
#include <random>
#include <string>
#include <utility>
#include <vector>

using lookup_fn = int (*)(std::string const &);
using names_fn = void (*)(std::vector<std::string> &);
extern lookup_fn const lookups[];
extern names_fn const names[];
extern int const type_count;

struct Query { lookup_fn lookup; std::string s; };

static double best_ns(std::vector<Query> const & q, long & checksum)
{
    double best = 1e30;
    for (int pass = 0; pass < 5; ++pass)
    {
        long n = 0;
        auto start = std::chrono::steady_clock::now();
        double elapsed = 0;
        do
        {
            for (auto const & e : q)
                checksum += e.lookup(e.s);
            n += (long) q.size();
            elapsed = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
        } while (elapsed < 0.3);
        best = std::min(best, elapsed * 1e9 / (double) n);
    }
    return best;
}

int main()
{
    std::vector<Query> all;
    std::vector<Query> first_type;
    std::vector<Query> misses;
    for (int i = 0; i < type_count; ++i)
    {
        std::vector<std::string> v;
        names[i](v);
        for (auto const & s : v)
        {
            all.push_back({lookups[i], s});
            misses.push_back({lookups[i], s + "0"});
            if (i == 0)
                first_type.push_back({lookups[i], s});
        }
    }
    std::mt19937 rng{20261117};
    std::shuffle(all.begin(), all.end(), rng);
    std::shuffle(misses.begin(), misses.end(), rng);

    // long random sequences over each working set, so the branch predictor cannot learn the order
    auto sequence = [&](std::vector<Query> const & set)
    {
        std::vector<Query> q;
        size_t const length = std::max<size_t>(65536, set.size());
        std::uniform_int_distribution<size_t> pick{0, set.size() - 1};
        for (size_t i = 0; i < length; ++i)
            q.push_back(set[pick(rng)]);
        return q;
    };
    auto first = [&](size_t n)
        { return std::vector<Query>(all.begin(), all.begin() + std::min(n, all.size())); };

    std::vector<std::pair<std::string, std::vector<Query>>> cases{
        {"one variant repeated", first(1)},
        {"one type", first_type},
    };
    for (size_t n : {20, 200, 2000})
        if (n < all.size())
            cases.push_back({std::to_string(n) + " variants", first(n)});
    cases.push_back({"all variants", all});
    cases.push_back({"all variants, misses", misses});
    long checksum = 0;
    for (auto const & c : cases)
        std::printf("%s\t%.1f\n", c.first.c_str(), best_ns(sequence(c.second), checksum));
    std::printf("checksum\t%ld\n", checksum);
}
'''


def usage():
    print(sys.argv[0] + ' [OPTION]...')
    print('times variants_by_string_index_of() in ns per call, from one variant looked up over and over to')
    print('random variants of many types, so cache and TLB misses show as well as the search itself, and')
    print('strings that are no variant (misses). Each case looks up a long random sequence drawn from its')
    print('set of variants, so the branch predictor cannot learn the order')
    print('    -h, --help            print this message')
    print('    -c  --clang           force use of clang compiler (uses default compiler if not set)')
    print('    -t  --types           number of types, one per translation unit (default 1000)')
    print('    -v  --variants        variants per type (default 50)')
    print('    -j  --jobs            max build jobs')
    print('                            * default is cpu count ['
          + str(multiprocessing.cpu_count()) + ']')
    print('    -r  --repeat          runs per program, the fastest is reported per case (default 3)')
    print('    -p  --pin             run the timed program on this cpu only (linux)')
    print('    -b  --baseline        include directory holding another matchable/matchable.h')
    print('                            * a second program is built with it, runs alternate (taking turns')
    print('                              going first) and the times are compared')
    print('    -w  --work_dir        where the programs are generated and built')
    print('                            * defaults to <matchable root>/build_bench_lookup')
    print('        --sweep           comma separated variants per type, e.g. 10,25,50,100,250,500,1000')
    print('                            * one program per value, keeping types x variants (-t x -v) the')
    print('                              same, so every point looks up the same number of variants')
    print('                            * each value must divide -t x -v exactly')
    print('                            * results go to a csv file for scripts/plot_sweep.py')
    print('    -o  --output          csv file for --sweep (default <work_dir>/sweep.csv)')
    print('\ntimes are only comparable with times from the same machine and load, so compare within')
    print('one run (ratios), not with numbers from elsewhere')


def generate(project_dir, types, variants):
    src_dir = project_dir + '/src'
    shutil.rmtree(project_dir, ignore_errors=True)
    os.makedirs(src_dir)
    with open(project_dir + '/CMakeLists.txt', 'w') as f:
        f.write(cmake_content)
    rng = random.Random(20261117)
    table = '#include <string>\n#include <vector>\n\n'
    for i in range(types):
        type_name = 'T' + str(i)
        content = matchable_source(type_name, variant_names(variants, rng))
        content += '\nvoid names_' + type_name + '(std::vector<std::string> & v)\n{\n'
        content += '    for (auto const & m : ' + type_name + '::variants_by_string())\n'
        content += '        v.push_back(m.as_string());\n}\n'
        with open(src_dir + '/' + type_name + '.cpp', 'w') as f:
            f.write(content)
        table += 'int lookup_' + type_name + '(std::string const &);\n'
        table += 'void names_' + type_name + '(std::vector<std::string> &);\n'
    table += '\nusing lookup_fn = int (*)(std::string const &);\n'
    table += 'using names_fn = void (*)(std::vector<std::string> &);\n'
    table += 'extern lookup_fn const lookups[];\nextern names_fn const names[];\n'
    table += 'extern int const type_count;\n'
    table += 'lookup_fn const lookups[] = {\n'
    table += ''.join('    lookup_T' + str(i) + ',\n' for i in range(types)) + '};\n'
    table += 'names_fn const names[] = {\n'
    table += ''.join('    names_T' + str(i) + ',\n' for i in range(types)) + '};\n'
    table += 'int const type_count = ' + str(types) + ';\n'
    with open(src_dir + '/table.cpp', 'w') as f:
        f.write(table)
    with open(src_dir + '/main.cpp', 'w') as f:
        f.write(main_content)


def build(project_dir, include_dir, use_clang, jobs):
    build_dir = project_dir + '/build'
    os.makedirs(build_dir)
    cmake_cmd = ['cmake', '-S', project_dir, '-B', build_dir, '-DCMAKE_BUILD_TYPE=Release',
                 '-DMATCHABLE_INCLUDE=' + include_dir]
    if use_clang:
        clang_c = which('clang')
        clang_cxx = which('clang++')
        if clang_c is None or clang_cxx is None:
            print('search for clang compiler failed')
            exit(1)
        cmake_cmd.append('-DCMAKE_C_COMPILER=' + clang_c)
        cmake_cmd.append('-DCMAKE_CXX_COMPILER=' + clang_cxx)
    if subprocess.run(cmake_cmd, stdout=subprocess.DEVNULL).returncode != 0:
        print('cmake failed')
        exit(1)
    if subprocess.run(['cmake', '--build', build_dir, '--parallel', str(jobs)],
                      stdout=subprocess.DEVNULL).returncode != 0:
        print('build failed')
        exit(1)
    return build_dir + '/bench_lookup'


def run(program, pin):
    pre = (lambda: os.sched_setaffinity(0, {pin})) if pin is not None else None
    result = subprocess.run([program], stdout=subprocess.PIPE, universal_newlines=True, preexec_fn=pre)
    if result.returncode != 0:
        print(program + ' failed with exit code ' + str(result.returncode))
        exit(1)
    rows = [line.split('\t') for line in result.stdout.splitlines()]
    return [(r[0], float(r[1])) for r in rows if r[0] != 'checksum']


def main():
    try:
        opts, args = getopt.getopt(sys.argv[1:], 'hct:v:j:r:p:b:w:o:',
                ['help', 'clang', 'types=', 'variants=', 'jobs=', 'repeat=', 'pin=', 'baseline=',
                 'work_dir=', 'sweep=', 'output='])
    except getopt.GetoptError as err:
        print(err)
        usage()
        sys.exit(2)

    use_clang = False
    types = 1000
    variants = 50
    jobs = multiprocessing.cpu_count()
    repeat = 3
    pin = None
    baseline = None
    work_dir = repo_root + 'build_bench_lookup'
    sweep = None
    output = None
    for o, a in opts:
        if o in ('-h', '--help'):
            usage()
            sys.exit()
        elif o in ('-c', '--clang'):
            use_clang = True
        elif o in ('-t', '--types'):
            types = int(a)
        elif o in ('-v', '--variants'):
            variants = int(a)
        elif o in ('-j', '--jobs'):
            jobs = int(a)
        elif o in ('-r', '--repeat'):
            repeat = int(a)
            if repeat < 1:
                print('--repeat needs at least 1')
                sys.exit(2)
        elif o in ('-p', '--pin'):
            pin = int(a)
            if not hasattr(os, 'sched_getaffinity') or pin not in os.sched_getaffinity(0):
                print('cannot pin to cpu ' + a)
                sys.exit(2)
        elif o in ('-b', '--baseline'):
            baseline = os.path.abspath(a)
            if not os.path.isfile(baseline + '/matchable/matchable.h'):
                print('no matchable/matchable.h in ' + baseline)
                sys.exit(2)
        elif o in ('-w', '--work_dir'):
            work_dir = os.path.abspath(a)
        elif o == '--sweep':
            sweep = parse_sweep(a)
        elif o in ('-o', '--output'):
            output = os.path.abspath(a)
        else:
            assert False, "unhandled option"

    sides = [('current', os.path.abspath(repo_root + 'include'))]
    if baseline is not None:
        sides.append(('baseline', baseline))

    if output is not None and sweep is None:
        print('--output is only used with --sweep')
        sys.exit(2)
    if sweep is not None:
        uneven = [v for v in sweep if (types * variants) % v != 0]
        if uneven:
            print('--sweep values must divide ' + str(types * variants) + ' (-t x -v) exactly: '
                  + ', '.join(str(v) for v in uneven) + ' do not')
            sys.exit(2)

    if sweep is None:
        cases, best = measure(sides, types, variants, work_dir, use_clang, jobs, repeat, pin)
        print_summary(cases, best, repeat, pin)
        return

    # each point is written as soon as it is measured, so stopping early keeps the finished points
    total = types * variants
    path = output or work_dir + '/sweep.csv'
    os.makedirs(os.path.dirname(path), exist_ok=True)
    start_csv(path)
    for v in sweep:
        t = total // v
        cases, best = measure(sides, t, v, work_dir, use_clang, jobs, repeat, pin)
        print_summary(cases, best, repeat, pin)
        append_csv(path, [['lookup', v, t, c, side, '{:.1f}'.format(best[side][c])]
                          for side, _ in sides for c in cases])
    print('\nwrote ' + path)


def measure(sides, types, variants, work_dir, use_clang, jobs, repeat, pin):
    programs = {}
    for side, include_dir in sides:
        print('building: ' + side + ' (' + str(types) + ' types x ' + str(variants) + ' variants)',
              flush=True)
        project_dir = work_dir + '/' + side
        generate(project_dir, types, variants)
        programs[side] = build(project_dir, include_dir, use_clang, jobs)

    # best[side][case] = ns
    best = {side: {} for side, _ in sides}
    cases = []
    for r in range(repeat):
        for side, _ in (sides if r % 2 == 0 else sides[::-1]):
            print('running: ' + side + ' ' + str(r + 1) + '/' + str(repeat), flush=True)
            for name, ns in run(programs[side], pin):
                if name not in cases:
                    cases.append(name)
                best[side][name] = min(best[side].get(name, ns), ns)
    return cases, best


def print_summary(cases, best, repeat, pin):
    print('\n\n******* Lookup Performance Summary *******')
    print('ns per variants_by_string_index_of(), fastest of ' + str(repeat) + ' run(s)'
          + ('' if pin is None else ', cpu ' + str(pin)) + '\n')
    width = max(len(c) for c in cases)
    for c in cases:
        ns = best['current'][c]
        line = '  ' + c.rjust(width) + ': ' + '{:8.1f}'.format(ns)
        if 'baseline' in best:
            b_ns = best['baseline'][c]
            line += '   baseline ' + '{:8.1f}'.format(b_ns) + '   ratio ' + '{:5.2f}'.format(ns / b_ns)
        print(line)


if __name__ == "__main__":
    main()
