cmake_minimum_required(VERSION 3.13)

project(matchable VERSION 10.2.1)

option(LIB_ONLY "Build library only (no tests/examples)" OFF)
option(OMIT_BY_INDEX "Omit by index" OFF)

if(OMIT_BY_INDEX)
    add_definitions(-DMATCHABLE_OMIT_BY_INDEX)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra")

install(DIRECTORY include/matchable DESTINATION include)



####### Testing ############################################################################################
if(NOT LIB_ONLY)
    # test system
    install(PROGRAMS test_scripts/run_all.sh DESTINATION share/matchable/test/bin/)
    install(PROGRAMS test_scripts/run_test.sh DESTINATION share/matchable/test/bin/)
    install(PROGRAMS test_scripts/list.sh DESTINATION share/matchable/test/bin/)

    # tests
    function(add_matchable_test test_name)
        add_executable(${test_name} test/programs/${test_name}.cpp)
        target_include_directories(${test_name} PRIVATE include)
        target_include_directories(${test_name} PRIVATE test/include)
        target_include_directories(${test_name} PUBLIC "${PROJECT_BINARY_DIR}")
        install(TARGETS ${test_name} RUNTIME DESTINATION share/matchable/test/programs)
    endfunction()
    add_matchable_test(escapable)
    add_matchable_test(self_property)
    add_matchable_test(matchable)
    add_matchable_test(matchbox)
    add_matchable_test(sorting)
    if(NOT OMIT_BY_INDEX)
        add_matchable_test(cards)
        add_matchable_test(matchable_usage)
        add_matchable_test(relationships)
        add_matchable_test(max_variants)
        add_matchable_test(unmatchable)
    endif()
endif()
############################################################################################################



####### libmatchable #######################################################################################
# create both static and shared libraries
# to avoid compiling twice first create a reusable "object" target

# reusable "object"
add_library(matchablemaker_object OBJECT src/MatchableMaker.cpp)
target_include_directories(matchablemaker_object PUBLIC include)
target_include_directories(matchablemaker_object PUBLIC "${PROJECT_BINARY_DIR}")
set_target_properties(matchablemaker_object PROPERTIES POSITION_INDEPENDENT_CODE 1)

# as shared library
add_library(matchablemaker_shared SHARED $<TARGET_OBJECTS:matchablemaker_object>)
set_target_properties(matchablemaker_shared PROPERTIES OUTPUT_NAME matchable CLEAN_DIRECT_OUTPUT 1)
target_include_directories(matchablemaker_object PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(
    matchablemaker_shared
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# as static library
add_library(matchablemaker_static STATIC $<TARGET_OBJECTS:matchablemaker_object>)
set_target_properties(matchablemaker_static PROPERTIES OUTPUT_NAME matchable CLEAN_DIRECT_OUTPUT 1)
target_include_directories(matchablemaker_object PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(
    matchablemaker_static
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

install(
    TARGETS matchablemaker_shared matchablemaker_static
    DESTINATION lib/matchable
    EXPORT matchable
)
############################################################################################################



####### play nice with others ##############################################################################
install(EXPORT matchable FILE matchable.cmake DESTINATION lib/matchable/cmake)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/matchable-config.cmake"
    INSTALL_DESTINATION "lib/matchable/cmake"
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/matchable-config-version.cmake"
    VERSION "${matchable_VERSION_MAJOR}.${matchable_VERSION_MINOR}.${matchable_VERSION_PATCH}"
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/matchable-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/matchable-config-version.cmake
    DESTINATION lib/matchable/cmake
)
############################################################################################################
