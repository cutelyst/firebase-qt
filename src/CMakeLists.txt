# SPDX-FileCopyrightText: (C) 2022 Daniel Nicoletti <dantti12@gmail.com>
# SPDX-License-Identifier: MIT

set(firebase_qt_SRC
    firebaseqtapp.cpp
    firebaseqtabstractmodule.cpp
)

set(firebase_qt_HEADERS
    firebaseqtapp.h
)

set(firebase_qt_HEADERS_PRIVATE
    firebaseqtapp_p.h
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
        firebase_messaging
)

set_property(TARGET ${target_app} PROPERTY PUBLIC_HEADER ${firebase_qt_HEADERS})
install(TARGETS ${target_app}
    EXPORT FirebaseTargets DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION bin COMPONENT runtime
    ARCHIVE DESTINATION lib COMPONENT devel
    PUBLIC_HEADER DESTINATION include/firebase-qt${QT_VERSION_MAJOR}/FirebaseQt COMPONENT devel
)

## Auth
set(firebase_qt_auth_SRC
    firebaseqtauth.cpp
    firebaseqtauthcredential.cpp
    firebaseqtauthphone.cpp
)

set(firebase_qt_auth_HEADERS
    firebaseqtauthcredential.h
    firebaseqtauth.h
    firebaseqtauthphone.h
)

set(firebase_qt_auth_HEADERS_PRIVATE
    firebaseqtauthcredential_p.h
)

set(target_auth FirebaseQt${QT_VERSION_MAJOR}_Auth)
add_library(${target_auth}
    ${firebase_qt_auth_SRC}
    ${firebase_qt_auth_HEADERS}
    ${firebase_qt_auth_HEADERS_PRIVATE}
)

#Add an alias so that library can be used inside the build tree, e.g. when testing
add_library(FirebaseQt::Auth ALIAS ${target_auth})

if (CMAKE_GENERATOR MATCHES "Visual Studio")
  set_property(TARGET ${target_auth} PROPERTY DEBUG_POSTFIX "d")
endif()

target_compile_definitions(${target_auth}
  PRIVATE
    PLUGINS_PREFER_DEBUG_POSTFIX=$<CONFIG:Debug>
)

target_include_directories(${target_auth} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include/firebase-qt${QT_VERSION_MAJOR}/FirebaseQt>
)

set_target_properties(${target_auth} PROPERTIES
    EXPORT_NAME Auth
    VERSION ${PROJECT_VERSION}
    SOVERSION 0
)

target_link_libraries(${target_auth}
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        FirebaseQt::App
        firebase_auth
)

set_property(TARGET ${target_auth} PROPERTY PUBLIC_HEADER ${firebase_qt_auth_HEADERS})
install(TARGETS ${target_auth}
    EXPORT FirebaseTargets DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION bin COMPONENT runtime
    ARCHIVE DESTINATION lib COMPONENT devel
    PUBLIC_HEADER DESTINATION include/firebase-qt${QT_VERSION_MAJOR}/FirebaseQt COMPONENT devel
)

### Messaging
set(firebase_qt_messaging_SRC
    firebaseqtmessaging.cpp
)

set(firebase_qt_messaging_HEADERS
    firebaseqtmessaging.h
)

set(firebase_qt_messaging_HEADERS_PRIVATE
)

set(target_messaging FirebaseQt${QT_VERSION_MAJOR}_Messaging)
add_library(${target_messaging}
    ${firebase_qt_messaging_SRC}
    ${firebase_qt_messaging_HEADERS}
    ${firebase_qt_messaging_HEADERS_PRIVATE}
)

#Add an alias so that library can be used inside the build tree, e.g. when testing
add_library(FirebaseQt::Messaging ALIAS ${target_messaging})

if (CMAKE_GENERATOR MATCHES "Visual Studio")
  set_property(TARGET ${target_messaging} PROPERTY DEBUG_POSTFIX "d")
endif()

target_compile_definitions(${target_messaging}
  PRIVATE
    PLUGINS_PREFER_DEBUG_POSTFIX=$<CONFIG:Debug>
)

target_include_directories(${target_messaging} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include/firebase-qt${QT_VERSION_MAJOR}/FirebaseQt>
)

set_target_properties(${target_messaging} PROPERTIES
    EXPORT_NAME Messaging
    VERSION ${PROJECT_VERSION}
    SOVERSION 0
)

target_link_libraries(${target_messaging}
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        FirebaseQt::App
        firebase_messaging
)

set_property(TARGET ${target_messaging} PROPERTY PUBLIC_HEADER ${firebase_qt_HEADERS})
install(TARGETS ${target_messaging}
    EXPORT FirebaseTargets DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION bin COMPONENT runtime
    ARCHIVE DESTINATION lib COMPONENT devel
    PUBLIC_HEADER DESTINATION include/firebase-qt${QT_VERSION_MAJOR}/FirebaseQt COMPONENT devel
)
