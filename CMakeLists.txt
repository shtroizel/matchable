cmake_minimum_required(VERSION 3.16.3)

project(matchable VERSION 15.1.0)

option(LIB_ONLY "Build library only (no tests/examples)" OFF)
option(OMIT_BY_INDEX "Omit by index" OFF)
option(SANITIZE "Build with AddressSanitizer and UndefinedBehaviorSanitizer" OFF)

if(OMIT_BY_INDEX)
    add_definitions(-DMATCHABLE_OMIT_BY_INDEX)
endif()

if(SANITIZE)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        message(FATAL_ERROR "SANITIZE needs gcc or clang")
    endif()
    add_compile_options(-fsanitize=address,undefined -fno-sanitize-recover=all -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:preprocessor")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-dangling-reference")
    endif()
endif()

include_directories(include)
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
        target_link_libraries(${test_name} PRIVATE ${ARGN})
        install(TARGETS ${test_name} RUNTIME DESTINATION share/matchable/test/programs)
    endfunction()

    # sources are in link order, which decides static initialization order, so keep them unsorted
    function(add_matchable_multi_tu_test test_name)
        set(srcs "")
        foreach(src ${ARGN})
            list(APPEND srcs test/programs/multi_tu/${src})
        endforeach()
        add_executable(${test_name} ${srcs})
        target_include_directories(${test_name} PRIVATE include)
        target_include_directories(${test_name} PRIVATE test/include)
        target_include_directories(${test_name} PUBLIC "${PROJECT_BINARY_DIR}")
        install(TARGETS ${test_name} RUNTIME DESTINATION share/matchable/test/programs)
    endfunction()
    add_matchable_multi_tu_test(multi_tu_main_first main.cpp a.cpp b.cpp plain.cpp)
    add_matchable_multi_tu_test(multi_tu_main_middle a.cpp plain.cpp main.cpp b.cpp)
    add_matchable_multi_tu_test(multi_tu_main_last plain.cpp a.cpp b.cpp main.cpp)

    # units loaded and unloaded at run time, built with MATCHABLE_UNLOADABLE; the program exports its
    # symbols so the units share its registry, and gcc needs -fno-gnu-unique for dlclose to unload a unit
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        add_executable(grow_unload test/programs/grow_unload/main.cpp)
        target_include_directories(grow_unload PRIVATE include test/include)
        # optimized, the program inlines property storage instead of exporting it, so a unit runs its own
        # copy and is the first to build storage the program goes on using
        target_compile_options(grow_unload PRIVATE -O2)
        set_target_properties(grow_unload PROPERTIES ENABLE_EXPORTS ON)
        target_link_libraries(grow_unload PRIVATE ${CMAKE_DL_LIBS})
        install(TARGETS grow_unload RUNTIME DESTINATION share/matchable/test/programs)
        foreach(version 1 2)
            add_library(grow_unload_unit_${version} MODULE test/programs/grow_unload/unit.cpp)
            target_include_directories(grow_unload_unit_${version} PRIVATE include)
            target_compile_definitions(grow_unload_unit_${version} PRIVATE
                UNIT_VERSION=${version} MATCHABLE_UNLOADABLE)
            if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
                target_compile_options(grow_unload_unit_${version} PRIVATE -fno-gnu-unique)
            endif()
            add_dependencies(grow_unload grow_unload_unit_${version})
            install(TARGETS grow_unload_unit_${version} LIBRARY DESTINATION share/matchable/test/modules)
        endforeach()
    endif()

    add_matchable_test(escapable)
    add_matchable_test(test_ok)
    add_matchable_test(self_property)
    add_matchable_test(matchable)
    add_matchable_test(matchbox)
    add_matchable_test(observer)
    add_matchable_test(sorting)
    add_matchable_test(property_matchable)
    add_matchable_test(observer_notify)
    add_matchable_test(flags_equality)
    add_matchable_test(currently_set)
    add_matchable_test(matchbox_nil)
    add_matchable_test(property_access_no_alloc)
    add_matchable_test(match_strict)
    add_matchable_test(fwd_header)
    add_matchable_test(lookup_by_string)
    add_matchable_test(matchable_maker matchablemaker_static)
    add_matchable_test(version)
    target_compile_definitions(version PRIVATE
        CMAKE_VERSION_MAJOR=${matchable_VERSION_MAJOR}
        CMAKE_VERSION_MINOR=${matchable_VERSION_MINOR}
        CMAKE_VERSION_PATCH=${matchable_VERSION_PATCH}
    )
    if(NOT OMIT_BY_INDEX)
        add_matchable_test(cards)
        add_matchable_test(matchable_usage)
        add_matchable_test(relationships)
        add_matchable_test(max_variants)
    endif()
endif()
############################################################################################################



####### libmatchable #######################################################################################
add_library(matchablemaker_shared SHARED src/MatchableMaker.cpp)
install(
    TARGETS matchablemaker_shared
    DESTINATION lib/matchable
    EXPORT matchable
)

add_library(matchablemaker_static STATIC src/MatchableMaker.cpp)
install(
    TARGETS matchablemaker_static
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
