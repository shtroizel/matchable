cmake_minimum_required(VERSION 3.0)

project(matchable)

set(CMAKE_CXX_STANDARD 20)

# comment next line out to avoid forcing clang
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra")


####### test scripts  #######
install(PROGRAMS scripts/run_all.sh DESTINATION test/bin/)
install(PROGRAMS scripts/run_test.sh DESTINATION test/bin/)
install(PROGRAMS scripts/list.sh DESTINATION test/bin/)


####### test programs #######
function(add_matchable_test test_name)
    add_executable(${test_name} test/programs/${test_name}.cpp)
    target_include_directories(${test_name} PRIVATE src)
    target_include_directories(${test_name} PRIVATE test/include)
    install(TARGETS ${test_name} RUNTIME DESTINATION test/programs)
endfunction()

add_matchable_test(max_variants)
add_matchable_test(matchable)
add_matchable_test(matchbox)
add_matchable_test(getting_started)
add_matchable_test(spread_matchable)
