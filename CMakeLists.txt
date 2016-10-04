###############################################################################
## CMake configuration

cmake_minimum_required(VERSION 2.8.12)

# Project name
project(Minesweeper)

# Limit the build configurations
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo"
    CACHE STRING "Limited configurations" FORCE)
mark_as_advanced(CMAKE_CONFIGURATION_TYPES)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release
        CACHE STRING "Choose the type of build, options are: Debug Release"
        FORCE)
endif()

# Set the custom module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

###############################################################################
## Project configuration

if(WIN32)
    set(HIDE_CMD TRUE CACHE BOOL "Hide the Windows CMD.")
endif()

###############################################################################
## Compiler configuration

# Adding C++11 support in GCC compiler and Clang compiler
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libstdc++")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()
# If compiler is GNU GCC or Clang enable the warnings
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

###############################################################################
## Directories configuration

# Output directories
string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${BUILD_TYPE}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${BUILD_TYPE}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${BUILD_TYPE}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/debug")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/debug")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/release")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/release")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/release")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/relwithdebinfo")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/relwithdebinfo")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/relwithdebinfo")

set(GAME_DIR "${CMAKE_SOURCE_DIR}/src")
set(THIRD_PARTY_DIR "${CMAKE_SOURCE_DIR}/third_party")
set(TEMP_DIR "${CMAKE_BINARY_DIR}/obj")

###############################################################################
## Build/find the third party libraries

# Finding the libraries
if(WIN32)
    find_package(SFML 2 COMPONENTS main graphics window system)
else()
    find_package(SFML 2 COMPONENTS graphics window system)
endif()
if(${SFML_FOUND})
    include_directories(${SFML_INCLUDE_DIR})
else()
    message(STATUS "Building SFML from source.")
    set(SFML_DIRECTORY ${THIRD_PARTY_DIR}/SFML)

    set(SFML_INCLUDE_DIR ${SFML_DIRECTORY}/include)

    set(BUILD_SHARED_LIBS FALSE)
    add_definitions(-DSFML_STATIC)

    add_subdirectory(${SFML_DIRECTORY} ${TEMP_DIR}/SFML EXCLUDE_FROM_ALL)
    if(WIN32)
        set(SFML_LIBRARIES sfml-main sfml-graphics sfml-window sfml-system)
    else()
        set(SFML_LIBRARIES sfml-graphics sfml-window sfml-system)
    endif()

    include_directories(${SFML_INCLUDE_DIR})
endif()

###############################################################################
# Create the game executable

file(GLOB_RECURSE GAME_SOURCES "${GAME_DIR}/*.cpp")
file(GLOB_RECURSE GAME_HEADERS "${GAME_DIR}/*.hpp")

if(HIDE_CMD)
    add_executable(${PROJECT_NAME} WIN32 ${GAME_SOURCES} ${GAME_HEADERS})
else()
    add_executable(${PROJECT_NAME} ${GAME_SOURCES} ${GAME_HEADERS})
endif()

target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})

# Copy the assets folder
add_custom_command(
    TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets"
)
