cmake_minimum_required(VERSION 3.10.2)
project(libihex CXX)

set(LIBIHEX_VERSION_MAJOR "1" CACHE STRING "Major version")
set(LIBIHEX_VERSION_MINOR "0" CACHE STRING "Minor version")
set(LIBIHEX_VERSION_PATCH "0" CACHE STRING "Patch version")
set(LIBIHEX_VERSION_LABEL "" CACHE STRING "Label version")

if(${PROJECT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
    set(LIBIHEX_IS_SUBMODULE OFF)
    message(STATUS "${PROJECT_NAME}: Building standalone")
else()
    set(LIBIHEX_IS_SUBMODULE ON)
    message(STATUS "${PROJECT_NAME}: Building as submodule")
endif()

option(LIBIHEX_ENABLE_SHARED_BUILD "Build shared library instead of static" OFF)
option(LIBIHEX_ENABLE_COVERAGE "Enable coverage" OFF)
option(LIBIHEX_ENABLE_TESTS "Enable tests" OFF)
option(LIBIHEX_ENABLE_GTEST_SUBMODULE "Enable use of GoogleTest submodule" OFF)

if((NOT LIBIHEX_IS_SUBMODULE) AND LIBIHEX_ENABLE_TESTS AND (NOT LIBIHEX_ENABLE_GTEST_SUBMODULE))
    message(NOTICE "${PROJECT_NAME}: Building standalone with tests enabled enables LIBIHEX_ENABLE_GTEST_SUBMODULE option")
    message(NOTICE "${PROJECT_NAME}: Enable LIBIHEX_ENABLE_GTEST_SUBMODULE option manually to avoid this notice")
    set(LIBIHEX_ENABLE_GTEST_SUBMODULE ON)
endif()

set(LIBIHEX_GCC_FLAGS "-Wall -Wextra -Wpedantic -Werror" CACHE STRING "GCC/G++ compiler flags")
separate_arguments(LIBIHEX_GCC_FLAGS_LIST NATIVE_COMMAND "${LIBIHEX_GCC_FLAGS}")
set(LIBIHEX_GCC_FLAGS_LIST ${LIBIHEX_GCC_FLAGS_LIST} CACHE INTERNAL "GCC/G++ compiler flags (list)")

if(LIBIHEX_ENABLE_COVERAGE)
    if(NOT CMAKE_COMPILER_IS_GNUCXX)
        message(FATAL_ERROR "${PROJECT_NAME}: Coverage is only supported with GNU G++ compiler toolchain.")
    endif()

    if(NOT LIBIHEX_ENABLE_TESTS)
        message(FATAL_ERROR "${PROJECT_NAME}: Coverage is only supported with LIBIHEX_ENABLE_TESTS option enabled.")
    endif()

    find_program(GCOVR_PATH gcovr)
    if(NOT GCOVR_PATH)
        message(FATAL_ERROR "${PROJECT_NAME}: gcovr not found.")
    endif()

    if(NOT (CMAKE_BUILD_TYPE STREQUAL "Debug"))
        message(WARNING "${PROJECT_NAME}: Coverage with non-debug build may cause misleading results.")
    endif()

    set(LIBIHEX_COVERAGE_CXX_FLAGS -g -O0 -fprofile-arcs -ftest-coverage)
    set(LIBIHEX_COVERAGE_LINKER_FLAGS --coverage)
endif()

add_subdirectory(libihex)
add_subdirectory(external)
