cmake_minimum_required(VERSION 3.20)
project(PSVRX VERSION 0.1.0 LANGUAGES CXX)

option(PSVRX_BUILD_TESTS "Build PSVR-X core tests" ON)

add_library(psvrx_core
    src/stereo_frame.cpp
)
target_include_directories(psvrx_core PUBLIC include)
target_compile_features(psvrx_core PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(psvrx_core PRIVATE /W4 /permissive-)
else()
    target_compile_options(psvrx_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(PSVRX_BUILD_TESTS)
    enable_testing()
    add_executable(psvrx_core_tests tests/test_stereo_frame.cpp)
    target_link_libraries(psvrx_core_tests PRIVATE psvrx_core)
    add_test(NAME psvrx_core_tests COMMAND psvrx_core_tests)
endif()
