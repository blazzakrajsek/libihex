cmake_minimum_required(VERSION 3.10.2)

project(ihex_test CXX)

set(LIBIHEX_TEST_HEX_FILE_ASSET_PATH "${PROJECT_BINARY_DIR}/asset/" CACHE FILEPATH "Test hex file asset path")

set(TEST_SAMPLES
    asset/sample1.hex
    asset/sample2.hex
)

foreach(SAMPLE ${TEST_SAMPLES})
    configure_file(${SAMPLE} ${SAMPLE} COPYONLY)
endforeach()

configure_file(
    include/${PROJECT_NAME}/test_hex_file_assets.hpp.in
    include/${PROJECT_NAME}/test_hex_file_assets.hpp
    @ONLY
)

set(TEST_PRIVATE_HEADERS
    include/${PROJECT_NAME}/test_hex_address_impl.hpp
    include/${PROJECT_NAME}/test_hex_file_assets.hpp
    include/${PROJECT_NAME}/test_hex_file_impl.hpp
    include/${PROJECT_NAME}/test_hex_group_impl.hpp
    include/${PROJECT_NAME}/test_hex_record_impl.hpp
    include/${PROJECT_NAME}/test_hex_section_impl.hpp
)

set(TEST_SOURCES
    src/testapp.cpp
    src/test_hex_address.cpp
    src/test_hex_address_impl.cpp
    src/test_hex_file.cpp
    src/test_hex_file_impl.cpp
    src/test_hex_group.cpp
    src/test_hex_group_impl.cpp
    src/test_hex_record.cpp
    src/test_hex_record_impl.cpp
    src/test_hex_section.cpp
    src/test_hex_section_impl.cpp
)

add_executable(${PROJECT_NAME} ${TEST_SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    PRIVATE_HEADER "${TEST_PRIVATE_HEADERS}"
)

target_compile_options(${PROJECT_NAME} PRIVATE ${LIBIHEX_GCC_FLAGS_LIST})

if(LIBIHEX_ENABLE_COVERAGE)
    target_compile_options(${PROJECT_NAME} PRIVATE ${LIBIHEX_COVERAGE_CXX_FLAGS})
    target_link_options(${PROJECT_NAME} PRIVATE ${LIBIHEX_COVERAGE_LINKER_FLAGS})
endif()

target_link_libraries(${PROJECT_NAME}
    PRIVATE gtest
    PRIVATE gtest_main
    PRIVATE LibIHex::LibIHex
)

target_include_directories(${PROJECT_NAME}
    PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
)

install(TARGETS ${PROJECT_NAME}
    PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
)
