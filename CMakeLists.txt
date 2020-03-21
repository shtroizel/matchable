cmake_minimum_required(VERSION 3.0)

project(matchable)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra")

install(FILES src/matchable.h DESTINATION include)

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
add_matchable_test(matchables_merge_spreads)
add_matchable_test(matchable_usage)
add_matchable_test(matchbox)
add_matchable_test(relationships)
add_matchable_test(spread_matchable)
add_matchable_test(spreadx2_matchable)
add_matchable_test(spreadvectof_matchable)
add_matchable_test(unmatchable)
