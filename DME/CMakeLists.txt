cmake_minimum_required(VERSION 3.16)

# ==============================
# Project Definition
# ==============================
project(DSA_Master_Engine
        VERSION 1.0
        DESCRIPTION "A complete DSA system-level engine"
        LANGUAGES CXX)

# ==============================
# Compiler Settings
# ==============================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable warnings (Google-style discipline)
if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ==============================
# Directory Variables
# ==============================
set(SRC_DIR ${PROJECT_SOURCE_DIR})
set(CORE_DIR ${SRC_DIR}/core)
set(DS_DIR ${SRC_DIR}/data_structures)
set(ALGO_DIR ${SRC_DIR}/algorithms)
set(SIM_DIR ${SRC_DIR}/simulations)
set(TEST_DIR ${SRC_DIR}/tests)
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
# Core System Sources
# ==============================
file(GLOB_RECURSE CORE_SOURCES
    ${CORE_DIR}/*.cpp
)

# ==============================
# Data Structures Sources
# ==============================
file(GLOB_RECURSE DS_SOURCES
    ${DS_DIR}/*.cpp
)

# ==============================
# Algorithms Sources
# ==============================
file(GLOB_RECURSE ALGO_SOURCES
    ${ALGO_DIR}/*.cpp
)

# ==============================
# Simulations Sources
# ==============================
file(GLOB_RECURSE SIM_SOURCES
    ${SIM_DIR}/*.cpp
)

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
# Build Type Defaults
# ==============================
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# ==============================
# Optimization Flags
# ==============================
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-O3)
else()
    add_compile_options(-O0 -g)
endif()

# ==============================
# Enable Testing
# ==============================
enable_testing()

# ==============================
# Test Executable
# ==============================
file(GLOB TEST_SOURCES
    ${TEST_DIR}/*.cpp
)

add_executable(dsa_tests
    ${TEST_SOURCES}
    ${CORE_SOURCES}
    ${DS_SOURCES}
    ${ALGO_SOURCES}
)

add_test(NAME RunAllTests COMMAND dsa_tests)

# ==============================
# Output Directories
# ==============================
set_target_properties(dsa_engine dsa_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build
)
