# SPDX-FileCopyrightText: (C) 2022-2026 Daniel Nicoletti <dantti12@gmail.com>
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.30)

# We need Swift LANGUAGE but if we add it in project
# clang++ fails to build due no_warn_duplicate_libraries being an
# undefined flag
project(FirebaseQt
    VERSION 1.0.0
    LANGUAGES CXX
)

# Due Firebase C++ SDK
set(CMAKE_POLICY_VERSION_MINIMUM 3.10)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

# Include our cmake modules
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)

find_package(QT NAMES Qt6 COMPONENTS Core Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} 6.4 COMPONENTS Core Network REQUIRED)

if (DEFINED FIREBASE_CPP_SDK_DIR)
    # Add Firebase libraries to the target using the function from the SDK.
    add_subdirectory(${FIREBASE_CPP_SDK_DIR} libs EXCLUDE_FROM_ALL)
else()
    include(FetchContent)
    FetchContent_Declare(
        firebase_cpp_sdk
        URL https://dl.google.com/firebase/sdk/cpp/firebase_cpp_sdk_12.2.0.zip
    )

    # MakeAvailable will run the CMakeList.txt script that was downloaded
    # but firebase_cpp_sdk_SOURCE_DIR that should populate FIREBASE_CPP_SDK_DIR
    # before running that script is only defined after MakeAvailable
    # so we set it here, in the hope FETCHCONTENT_BASE_DIR is not changed.
    set(FIREBASE_CPP_SDK_DIR "${CMAKE_BINARY_DIR}/_deps/firebase_cpp_sdk-src")

    FetchContent_MakeAvailable(firebase_cpp_sdk)
endif()
message(STATUS "FIREBASE_CPP_SDK_DIR=${FIREBASE_CPP_SDK_DIR}")

if(IOS)
    if (NOT DEFINED FIREBASE_IOS_SDK_DIR)
        message(FATAL_ERROR "FIREBASE_IOS_SDK_DIR not defined")
    endif()
    message(STATUS "FIREBASE_IOS_SDK_DIR=${FIREBASE_IOS_SDK_DIR}")
endif()

add_subdirectory(src)
