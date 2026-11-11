cmake_minimum_required(VERSION 3.20)
project(vitalcomm
    VERSION 0.0.1
    DESCRIPTION "SIL-minded, railway-inspired message bus in Modern C++17"
    LANGUAGES CXX
)

# ---- Language & compile standards ----
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- Options ----
option(VITALCOMM_BUILD_TESTS    "Build unit and integration tests" ON)
option(VITALCOMM_BUILD_EXAMPLES "Build example binaries"           ON)
set(VITALCOMM_SANITIZE "" CACHE STRING
    "Comma-separated sanitizers to enable (address, thread, undefined)")

# ---- Warnings (strict, used by the library and tests) ----
add_library(vitalcomm_warnings INTERFACE)
target_compile_options(vitalcomm_warnings INTERFACE
    -Wall -Wextra -Wpedantic -Wshadow -Wconversion
    -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual
    -Wnon-virtual-dtor -Wnull-dereference -Wdouble-promotion
    -Wformat=2
)

# ---- Sanitizers (opt-in via VITALCOMM_SANITIZE) ----
if(VITALCOMM_SANITIZE)
    add_library(vitalcomm_sanitize INTERFACE)
    target_compile_options(vitalcomm_sanitize INTERFACE
        -fsanitize=${VITALCOMM_SANITIZE} -fno-omit-frame-pointer -g)
    target_link_options(vitalcomm_sanitize INTERFACE
        -fsanitize=${VITALCOMM_SANITIZE})
endif()

# ---- Library target ----
add_library(vitalcomm
    src/crc32.cpp
)
target_include_directories(vitalcomm PUBLIC include)
target_link_libraries(vitalcomm PRIVATE vitalcomm_warnings)
if(VITALCOMM_SANITIZE)
    target_link_libraries(vitalcomm PUBLIC vitalcomm_sanitize)
endif()

# ---- Tests ----
if(VITALCOMM_BUILD_TESTS)
    enable_testing()
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    add_subdirectory(tests)
endif()

# ---- Examples ----
if(VITALCOMM_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
