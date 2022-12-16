# SPDX-FileCopyrightText: (C) 2022 Daniel Nicoletti <dantti12@gmail.com>
# SPDX-License-Identifier: MIT

set(firebase_qt_SRC
    firebaseqtapp.cpp
    firebaseqtabstractmodule.cpp
    firebaseqtauth.cpp
    firebaseqtauthcredential.cpp
    firebaseqtauthphone.cpp
    firebaseqtmessaging.cpp
)

set(firebase_qt_HEADERS
    firebaseqtapp.h
    firebaseqtauthcredential.h
    firebaseqtauth.h
    firebaseqtauthphone.h
    firebaseqtmessaging.h
)

set(firebase_qt_HEADERS_PRIVATE
    firebaseqtapp_p.h
    firebaseqtauthcredential_p.h
)

set(target_app FirebaseQt${QT_VERSION_MAJOR}_App)
add_library(${target_app}
    ${firebase_qt_SRC}
    ${firebase_qt_HEADERS}
    ${firebase_qt_HEADERS_PRIVATE}
)

#Add an alias so that library can be used inside the build tree, e.g. when testing
add_library(FirebaseQt::App ALIAS ${target_app})

if (CMAKE_GENERATOR MATCHES "Visual Studio")
  set_property(TARGET ${target_app} PROPERTY DEBUG_POSTFIX "d")
endif()

target_compile_definitions(${target_app}
  PRIVATE
    PLUGINS_PREFER_DEBUG_POSTFIX=$<CONFIG:Debug>
)

target_include_directories(${target_app} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include/firebase-qt${QT_VERSION_MAJOR}/FirebaseQt>
)

set_target_properties(${target_app} PROPERTIES
    EXPORT_NAME App
    VERSION ${PROJECT_VERSION}
    SOVERSION 0
)

target_link_libraries(${target_app}
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::CorePrivate
        Qt${QT_VERSION_MAJOR}::Network
        firebase_analytics
        firebase_app
)

set_property(TARGET ${target_app} PROPERTY PUBLIC_HEADER ${firebase_qt_HEADERS})
install(TARGETS ${target_app}
    EXPORT FirebaseTargets DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION bin COMPONENT runtime
    ARCHIVE DESTINATION lib COMPONENT devel
    PUBLIC_HEADER DESTINATION include/firebase-qt${QT_VERSION_MAJOR}/FirebaseQt COMPONENT devel
)
