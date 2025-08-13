cmake_minimum_required(VERSION 3.18)
project(BacktrackDetector LANGUAGES CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(BacktrackDetector SHARED
    include/cssdk/public/interface.cpp
    src/dllapi.cpp
    src/engine_api.cpp
    src/ex_rehlds_api.cpp
    src/h_export.cpp
    src/meta_api.cpp
    src/backtrack_detector.cpp
    src/sdk_util.cpp
)

target_include_directories(BacktrackDetector PRIVATE 
    include
    include/cssdk/common
    include/cssdk/dlls
    include/cssdk/dlls/bot
    include/cssdk/dlls/hostage
    include/cssdk/engine
    include/cssdk/game_shared
    include/cssdk/game_shared/bot
    include/cssdk/pm_shared
    include/cssdk/public
    include/cssdk/public/cl_dll
    include/cssdk/public/HLTV
    include/cssdk/public/steam
    include/cssdk/public/tier0
    include/cssdk/public/vgui
    include/metamod
    src
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_definitions(-D_CRT_SECURE_NO_WARNINGS)
    target_link_options(BacktrackDetector PRIVATE /EXPORT:GiveFnptrsToDll=_GiveFnptrsToDll@8 /SECTION:.data,RW)
    target_compile_options(BacktrackDetector PRIVATE 
        /std:c++20
        /ZI
        /Zc:strictStrings-
    )
    set_target_properties(BacktrackDetector PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    )
    target_link_libraries(BacktrackDetector PRIVATE psapi.lib dbghelp.lib Ws2_32.lib)
else()
    target_compile_options(BacktrackDetector PRIVATE 
        -std=c++20
        -Wno-conversion-null
        -Wno-format
        -Wno-write-strings
        -fpermissive
        -D_vsnprintf=vsnprintf
        -m32
    )
    target_link_options(BacktrackDetector PRIVATE -m32)
    target_compile_features(BacktrackDetector PUBLIC cxx_std_20)
    set_target_properties(BacktrackDetector PROPERTIES 
        COMPILE_FLAGS "-m32"
        LINK_FLAGS "-m32"
        POSITION_INDEPENDENT_CODE ON
    )
    target_link_libraries(BacktrackDetector PRIVATE -static-libstdc++ -static-libgcc)
endif()

set_target_properties(BacktrackDetector PROPERTIES PREFIX "")

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set_target_properties(BacktrackDetector PROPERTIES OUTPUT_NAME "backtrack_detector_mm")
else()
    set_target_properties(BacktrackDetector PROPERTIES OUTPUT_NAME "backtrack_detector_mm_i386")
endif()