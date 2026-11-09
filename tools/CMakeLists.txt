cmake_minimum_required(VERSION 3.20)
project(PSVRXCameraProbe LANGUAGES CXX)

if(NOT WIN32)
    message(FATAL_ERROR "PSVR-X camera_probe is Windows-only (Media Foundation).")
endif()

add_executable(psvrx_camera_probe camera_probe.cpp)
target_compile_features(psvrx_camera_probe PRIVATE cxx_std_17)
target_compile_definitions(psvrx_camera_probe PRIVATE UNICODE _UNICODE)
target_link_libraries(psvrx_camera_probe PRIVATE
    mfplat
    mfreadwrite
    mfuuid
    ole32
)

if(MSVC)
    target_compile_options(psvrx_camera_probe PRIVATE /W4 /permissive-)
endif()
