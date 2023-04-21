cmake_minimum_required(VERSION 3.10.2)
project(ihex CXX)

configure_file(
    include/${PROJECT_NAME}/version.hpp.in
    include/${PROJECT_NAME}/version.hpp
    @ONLY
)

set(PROJECT_PUBLIC_HEADERS
    include/${PROJECT_NAME}/hex_address.hpp
    include/${PROJECT_NAME}/hex_file.hpp
    include/${PROJECT_NAME}/hex_group.hpp
    include/${PROJECT_NAME}/hex_record.hpp
    include/${PROJECT_NAME}/hex_section.hpp
    include/${PROJECT_NAME}/version.hpp
)

set(PROJECT_SOURCES
    src/hex_address.cpp
    src/hex_file.cpp
    src/hex_group.cpp
    src/hex_record.cpp
    src/hex_section.cpp
)

if (LIBIHEX_ENABLE_SHARED_BUILD)
    add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES})
else()
    add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCES})
endif()
add_library(LibIHex::LibIHex ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    PUBLIC_HEADER "${PROJECT_PUBLIC_HEADERS}"
)

target_compile_options(${PROJECT_NAME} PRIVATE ${LIBIHEX_GCC_FLAGS_LIST})

if(LIBIHEX_ENABLE_COVERAGE)
    target_compile_options(${PROJECT_NAME} PRIVATE ${LIBIHEX_COVERAGE_CXX_FLAGS})
    target_link_options(${PROJECT_NAME} PRIVATE ${LIBIHEX_COVERAGE_LINKER_FLAGS})
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PUBLIC $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
)

install(TARGETS ${PROJECT_NAME}
    PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
)

if(LIBIHEX_ENABLE_TESTS)
    add_subdirectory(test)

    if(LIBIHEX_ENABLE_COVERAGE)
        set(COVERAGE_TARGET "${PROJECT_NAME}_coverage")
        set(COVERAGE_OUTPUT_SUFFIX "coverage")
        set(COVERAGE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/${COVERAGE_OUTPUT_SUFFIX}")

        message(STATUS "${PROJECT_NAME}: Build ${COVERAGE_TARGET} target to generate coverage report.")

        add_custom_target(${COVERAGE_TARGET}
            # Delete existing gcda files
            COMMAND
                find ${PROJECT_BINARY_DIR} -iname *.gcda -delete

            # Make coverage directory
            COMMAND
                ${CMAKE_COMMAND} -E make_directory ${COVERAGE_OUTPUT_PATH}

            # Run test executable
            COMMAND
                ${PROJECT_BINARY_DIR}/test/${PROJECT_NAME}_test

            # Generate coverage report
            COMMAND
                ${GCOVR_PATH}
                -r ${PROJECT_SOURCE_DIR}
                ${PROJECT_BINARY_DIR}
                -e ${PROJECT_SOURCE_DIR}/test
                --exclude-unreachable-branches
                --exclude-throw-branches
                --html-title ${PROJECT_NAME}
                --html-details
                -o ${COVERAGE_OUTPUT_SUFFIX}/${PROJECT_NAME}.html

            BYPRODUCTS ${COVERAGE_OUTPUT_PATH}/${PROJECT_NAME}.html
            WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
            VERBATIM
            COMMENT "${PROJECT_NAME}: Coverage report generated at ${COVERAGE_OUTPUT_PATH}."
        )
    endif()
endif()
