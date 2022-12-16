# SPDX-FileCopyrightText: (C) 2022 Daniel Nicoletti <dantti12@gmail.com>
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.24)
project(FirebaseQt VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} 5.10.0 COMPONENTS Core Network REQUIRED)

include(FetchContent)
FetchContent_Declare(
  firebase_cpp_sdk
  URL https://dl.google.com/firebase/sdk/cpp/firebase_cpp_sdk_10.3.0.zip
)
FetchContent_MakeAvailable(firebase_cpp_sdk)

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

# Include our cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX
    "/usr" CACHE PATH "firebase admin default install prefix" FORCE)
endif()

add_definitions("-DLOCALSTATEDIR=\"${LOCALSTATEDIR}\"")

set(CMAKE_INSTALL_LIBDIR "${CMAKE_INSTALL_PREFIX}/lib/${CMAKE_LIBRARY_ARCHITECTURE}" CACHE PATH "Output directory for libraries")

option(BUILD_SHARED_LIBS "Build in shared lib mode" OFF)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/FirebaseQt
)

# cmake config files
configure_file(${CMAKE_MODULE_PATH}/firebase-qt-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/FirebaseQt${QT_VERSION_MAJOR}Config.cmake
    @ONLY
)
configure_file(${CMAKE_MODULE_PATH}/firebase-qt-config-version.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/FirebaseQt${QT_VERSION_MAJOR}ConfigVersion.cmake
    @ONLY
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/FirebaseQt${QT_VERSION_MAJOR}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/FirebaseQt${QT_VERSION_MAJOR}ConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/FirebaseQt${QT_VERSION_MAJOR}/
)

install(EXPORT FirebaseTargets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/FirebaseQt${QT_VERSION_MAJOR}/
    FILE Firebase${PROJECT_VERSION_MAJOR}Qt${QT_VERSION_MAJOR}Targets.cmake
    NAMESPACE FirebaseQt::
    COMPONENT Devel
)

add_subdirectory(src)
include(CPackConfig)
