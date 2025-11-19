cmake_minimum_required(VERSION 3.20.2)

project(elk-e VERSION 0.0.1 LANGUAGES C CXX )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake") # Out-of-src build in the cmake-dir

include(CMakePackageConfigHelpers)  # Supplied by cmake
include(GNUInstallDirs)             # Supplied by cmake
include(CheckTypeSize)              # Supplied by cmake
include(CheckSymbolExists)          # Supplied by cmake
include(CheckCXXCompilerFlag)       # Supplied by cmake
include(FetchContent)               # Supplied by cmake

CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)

set(BuildValues "Release;Debug;RelWithDebInfo;MinSizeRel;Native")
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
    message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})
endif()
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${BuildValues})

if (CMAKE_CXX_FLAGS_NATIVE STREQUAL "")
    set(CMAKE_CXX_FLAGS_NATIVE "-O3 -DNDEBUG" CACHE STRING "" FORCE)
    if(COMPILER_SUPPORTS_MARCH_NATIVE)
        set(CMAKE_CXX_FLAGS_NATIVE "${CMAKE_CXX_FLAGS_NATIVE} -march=native" CACHE STRING "" FORCE)
    endif()
endif()

#========================================================== Dependencies
find_package(MPI REQUIRED)
find_package(Lua 5.3 REQUIRED)

#========================================================== Compile options
set(Project_CXX_FLAGS)
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    list(APPEND Project_CXX_FLAGS "-pedantic")
    list(APPEND Project_CXX_FLAGS "-Wall")
    list(APPEND Project_CXX_FLAGS "-Wno-unused-variable")
    list(APPEND Project_CXX_FLAGS "-Wno-c11-extensions")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
    list(APPEND Project_CXX_FLAGS "-pedantic")
    list(APPEND Project_CXX_FLAGS "-Wall")
    list(APPEND Project_CXX_FLAGS "-Wno-unused-variable")
    list(APPEND Project_CXX_FLAGS "-Wno-c11-extensions")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    list(APPEND Project_CXX_FLAGS "-pedantic")
    list(APPEND Project_CXX_FLAGS "-Wall")
    list(APPEND Project_CXX_FLAGS "-Wno-unused-variable")
    list(APPEND Project_CXX_FLAGS "-Wno-sign-compare")
    list(APPEND Project_CXX_FLAGS "-Wno-psabi")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
    list(APPEND Project_CXX_FLAGS "-fp-model=precise")
else()
    message(WARNING "Untested compiler : ${CMAKE_CXX_COMPILER_ID}")
endif()

configure_file(${PROJECT_SOURCE_DIR}/tools/elke_configuration.h.in
               ${PROJECT_SOURCE_DIR}/elke_core/elke_configuration.h)

if (NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

    set(CONFIG_FILE_NAME "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.cmake")
    file(WRITE "${CONFIG_FILE_NAME}"
        "# Environment configuration file for project.\n")
endif()

#========================================================== Setup include directories
include_directories("${PROJECT_SOURCE_DIR}")

# libelke
file(GLOB_RECURSE libelke_SRCS CONFIGURE_DEPENDS
    elke_core/*.cc
    elke_modules/*.cc
)
list(REMOVE_ITEM libelke_SRCS "elke_main.cc")

add_library(elke_lib_objects OBJECT ${libelke_SRCS})
add_library(elke_lib_static STATIC)
add_library(elke_lib_shared SHARED)

#========================================================== External dependencies
if(EXISTS "${PROJECT_SOURCE_DIR}/external/yaml-cpp/CMakeLists.txt")
    message(STATUS "Submodule found: yaml-cpp")
    add_subdirectory("${PROJECT_SOURCE_DIR}/external/yaml-cpp/")
    include_directories("${PROJECT_SOURCE_DIR}/external/yaml-cpp/include")

    add_compile_definitions(YAML_CPP_EXISTS)
endif()


#========================================================== Main outputs
target_link_libraries(elke_lib_static PUBLIC elke_lib_objects yaml-cpp::yaml-cpp)
target_link_libraries(elke_lib_shared PUBLIC elke_lib_objects yaml-cpp::yaml-cpp)

# Test Executable
file(GLOB_RECURSE elke_test_SRCS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/test/src/*.cc")
add_executable(elke_test ${elke_test_SRCS} "${PROJECT_SOURCE_DIR}/elke_main.cc")

target_link_libraries(elke_test elke_lib_static)

# Executable
add_executable(elke "${PROJECT_SOURCE_DIR}/elke_main.cc")

target_link_libraries(elke elke_lib_static)

# |------------ Write Makefile to root directory
file(WRITE ${PROJECT_SOURCE_DIR}/Makefile "subsystem:\n" "\t$(MAKE) -C elke_build \n\n"
        "clean:\n\t$(MAKE) -C elke_build clean\n")
