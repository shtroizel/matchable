#!/usr/bin/env bash
# usage: cxx14.sh [-a]
# Builds and runs the tests that need only C++14 with ${CXX:-g++} -std=gnu++1y. -a omits by index.

set -u
cd "$(dirname "${0}")/.."

cxx=${CXX:-g++}
flags=(-std=gnu++1y -Wall -Wextra -Werror -Wno-attributes -Wno-unused-variable -Iinclude -Itest/include)
needs_cxx17=" escapable sorting matchable_maker "
needs_by_index=" cards matchable_usage relationships max_variants "
if ! "${cxx}" -dM -E -x c++ /dev/null | grep -q __clang__
then
    flags+=(-Wno-dangling-reference)
fi
if [[ "${1:-}" == "-a" ]]
then
    flags+=(-DMATCHABLE_OMIT_BY_INDEX)
fi

out=$(mktemp -d)
trap 'rm -rf "${out}"' EXIT
failed=()

run()
{
    local name=${1}
    shift
    echo "${name}"
    if ! "${cxx}" "${flags[@]}" "$@" -o "${out}/${name}"
    then
        failed+=("${name} (build)")
    elif ! "${out}/${name}" > "${out}/${name}.log" 2>&1
    then
        cat "${out}/${name}.log"
        failed+=("${name}")
    fi
}

# version compares matchable.h with the project version in CMakeLists.txt, which CMake passes it
version=$(sed -n 's/^project(matchable VERSION \([0-9.]*\))$/\1/p' CMakeLists.txt)
IFS=. read -r major minor patch <<< "${version}"
version_flags=(-DCMAKE_VERSION_MAJOR="${major}" -DCMAKE_VERSION_MINOR="${minor}"
               -DCMAKE_VERSION_PATCH="${patch}")

for src in test/programs/*.cpp
do
    name=$(basename "${src}" .cpp)
    [[ "${needs_cxx17}" == *" ${name} "* ]] && continue
    [[ "${1:-}" == "-a" && "${needs_by_index}" == *" ${name} "* ]] && continue
    if [[ "${name}" == "version" ]]
    then
        run "${name}" "${src}" "${version_flags[@]}"
    else
        run "${name}" "${src}"
    fi
done
run multi_tu test/programs/multi_tu/{main,a,b,plain}.cpp

"${cxx}" --version | head -n 1
if [ ${#failed[@]} -ne 0 ]
then
    echo "${#failed[@]} failed: ${failed[*]}"
    exit 1
fi
echo "All C++14 tests passed"
