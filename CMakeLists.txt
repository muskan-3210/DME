cmake_minimum_required(VERSION 3.16)

project(DSA_Master_Engine
        VERSION 1.0
        DESCRIPTION "A complete DSA system-level engine"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ==============================
# Compiler Warnings
# ==============================
if(MSVC)
    add_compile_options(/W3 /wd4996)
else()
    add_compile_options(-Wall -Wextra)
endif()

# ==============================
# Directory Variables
# ==============================
set(SRC_DIR    ${PROJECT_SOURCE_DIR})
set(CORE_DIR   ${SRC_DIR}/core)
set(DS_DIR     ${SRC_DIR}/data_structures)
set(ALGO_DIR   ${SRC_DIR}/algorithms)
set(SIM_DIR    ${SRC_DIR}/simulations)
set(TEST_DIR   ${SRC_DIR}/tests)
set(CONFIG_DIR ${SRC_DIR}/config)

# ==============================
# Include Paths
# ==============================
include_directories(
    ${SRC_DIR}
    ${CORE_DIR}
    ${DS_DIR}
    ${ALGO_DIR}
    ${SIM_DIR}
    ${CONFIG_DIR}
)

# ==============================
# Source Collections
# ==============================
file(GLOB_RECURSE CORE_SOURCES  ${CORE_DIR}/*.cpp)
file(GLOB_RECURSE DS_SOURCES    ${DS_DIR}/*.cpp)
file(GLOB_RECURSE ALGO_SOURCES  ${ALGO_DIR}/*.cpp)
file(GLOB_RECURSE SIM_SOURCES   ${SIM_DIR}/*.cpp)

# ==============================
# Main Executable
# ==============================
add_executable(dsa_engine
    main.cpp
    ${CORE_SOURCES}
    ${DS_SOURCES}
    ${ALGO_SOURCES}
    ${SIM_SOURCES}
)

# ==============================
# Optimization Flags
# ==============================
if(MSVC)
    target_compile_options(dsa_engine PRIVATE
        $<$<CONFIG:Release>:/O2>
        $<$<CONFIG:Debug>:/Od /Zi>
    )
else()
    target_compile_options(dsa_engine PRIVATE
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:Debug>:-O0 -g>
    )
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# ==============================
# Tests
# ==============================
enable_testing()

file(GLOB TEST_SOURCES ${TEST_DIR}/*.cpp)

# Filter to only include test files that define main()
set(VALID_TEST_SOURCES "")
foreach(src ${TEST_SOURCES})
    file(READ ${src} _content)
    string(FIND "${_content}" "int main" _pos)
    if(NOT _pos EQUAL -1)
        list(APPEND VALID_TEST_SOURCES ${src})
    endif()
endforeach()

if(VALID_TEST_SOURCES)
    add_executable(dsa_tests
        ${VALID_TEST_SOURCES}
        ${CORE_SOURCES}
        ${DS_SOURCES}
        ${ALGO_SOURCES}
    )
    if(MSVC)
        target_compile_options(dsa_tests PRIVATE /Od /Zi)
    else()
        target_compile_options(dsa_tests PRIVATE -O0 -g)
    endif()
    add_test(NAME RunAllTests COMMAND dsa_tests)
endif()

# ==============================
# Output Directory
# ==============================
set_target_properties(dsa_engine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build
)
