cmake_minimum_required(VERSION 3.20.2)

project(elk-e
    VERSION 0.0.1
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

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

# dependencies
find_package(MPI REQUIRED)
find_package(Lua 5.3 REQUIRED)

# find_package(VTK QUIET)
# if(VTK_VERSION VERSION_GREATER_EQUAL "9.0.0")
#     find_package(VTK
#         COMPONENTS
#             CommonCore
#             CommonDataModel
#             IOLegacy
#             IOCore
#             IOXML
#             ParallelCore
#             IOParallelXML
#             FiltersCore
#             IOEnSight
#             IOExodus
#         REQUIRED
#     )
#     message(STATUS "Found VTK: ${VTK_DIR} (found version \"${VTK_VERSION}\")")
# elseif(VTK_VERSION VERSION_GREATER_LESS "8.0.0")
#     message(FATAL_ERROR "Found VTK: ${VTK_DIR} (found version \"${VTK_VERSION}\")
# but the minimum version is 9.0.0")
# else()
#     if (${VTK_DIR} STREQUAL VTK_DIR-NOTFOUND)
#         message(FATAL_ERROR "Could not find VTK")
#     else()
#         message(FATAL_ERROR "Found unsupported version of VTK: ${VTK_DIR}, version ${VTK_VERSION}")
#     endif()
# endif()

# find_package(PETSc 3.20 REQUIRED)
# check_symbol_exists(PETSC_USE_64BIT_INDICES "${PETSC_INCLUDE_DIR}/petscconf.h" PETSC_USE_64BIT_INDICES)
# if(NOT ${PETSC_USE_64BIT_INDICES} MATCHES 1)
#     message(FATAL_ERROR "PETSc has not been configured with the flag --with-64-bit-indices\n")
# endif()
# check_symbol_exists(PETSC_HAVE_PTSCOTCH "${PETSC_INCLUDE_DIR}/petscconf.h" PETSC_HAVE_PTSCOTCH)
# if(NOT ${PETSC_HAVE_PTSCOTCH} MATCHES 1)
#     message(FATAL_ERROR "PETSc has not been configured with PTSCOTCH\n")
# endif()

# compile options
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

include_directories("${PROJECT_SOURCE_DIR}")

# libelke
file(GLOB_RECURSE libelke_SRCS CONFIGURE_DEPENDS
    elke_core/*.cc
    elke_modules/*.cc
)
list(REMOVE_ITEM libelke_SRCS "elke_main.cc")

add_library(libelke SHARED ${libelke_SRCS})

target_include_directories(libelke
    PRIVATE
    $<INSTALL_INTERFACE:include/elke>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/external
    PUBLIC
    $<BUILD_INTERFACE:${PETSC_INCLUDE_DIR}>
    $<BUILD_INTERFACE:${LUA_INCLUDE_DIRS}>
)

target_link_libraries(libelke
    PRIVATE
    ${PETSC_LIBRARY}
    ${VTK_LIBRARIES}
    MPI::MPI_CXX
)
target_link_libraries(libelke PRIVATE ${LUA_LIBRARIES})

target_compile_options(libelke PRIVATE ${Project_CXX_FLAGS})

if(NOT MSVC)
    set_target_properties(libelke PROPERTIES OUTPUT_NAME elke)
endif()

set_target_properties(
    libelke
    PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
)

# Test Executable
file(GLOB_RECURSE elke_test_SRCS CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/test/src/*.cc")
add_executable(elke_test ${elke_test_SRCS}
               "${PROJECT_SOURCE_DIR}/elke_main.cc")

target_link_libraries(elke_test libelke)

# Executable
add_executable(elke "${PROJECT_SOURCE_DIR}/elke_main.cc")

target_link_libraries(elke libelke)

# |------------ Write Makefile to root directory
file(WRITE ${PROJECT_SOURCE_DIR}/Makefile "subsystem:\n" "\t$(MAKE) -C elke_build \n\n"
        "clean:\n\t$(MAKE) -C elke_build clean\n")
