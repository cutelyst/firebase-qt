# SPDX-FileCopyrightText: (C) 2022-2026 Daniel Nicoletti <dantti12@gmail.com>
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

set(target_fb_app FirebaseQt${QT_VERSION_MAJOR}_App)
add_library(${target_fb_app} OBJECT
    ${firebase_qt_SRC}
    ${firebase_qt_HEADERS}
    ${firebase_qt_HEADERS_PRIVATE}
    ios.swift
)
add_library(FirebaseQt::App ALIAS ${target_fb_app})

target_link_libraries(${target_fb_app}
    PRIVATE
        Qt::Core
        Qt::Network
        firebase_app
)

# Auth
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
add_library(FirebaseQt::Auth ALIAS ${target_auth})

target_link_libraries(${target_auth}
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        firebase_auth
        FirebaseQt::App
)

# Messaging
set(firebase_qt_messaging_SRC
    firebaseqtmessaging.cpp
)

set(firebase_qt_messaging_HEADERS
    firebaseqtmessaging.h
)

set(firebase_qt_messaging_HEADERS_PRIVATE
)

set(target_messaging FirebaseQt${QT_VERSION_MAJOR}_Messaging)
add_library(${target_messaging} OBJECT
    ${firebase_qt_messaging_SRC}
    ${firebase_qt_messaging_HEADERS}
    ${firebase_qt_messaging_HEADERS_PRIVATE}
)
add_library(FirebaseQt::Messaging ALIAS ${target_messaging})

target_link_libraries(${target_messaging}
    PRIVATE
        Qt::Core
        firebase_messaging
        FirebaseQt::App
)

## iOS
if(NOT IOS)
    function(create_firebase_target target_name)
        # Get the arguments passed to the function
        set(dependencies ${ARGN})

        # Define the target as an imported library
        add_library(${target_name} STATIC IMPORTED)

        # Set properties for the imported target
        set_target_properties(${target_name} PROPERTIES
            IMPORTED_LOCATION "${FIREBASE_IOS_SDK_DIR}/${target_name}/${target_name}.xcframework"
        )

        # Loop over the dependencies and create imported targets for them
        foreach(dependency ${dependencies})
            # Define each dependency as an imported library
            add_library(${dependency} STATIC IMPORTED)

            # Set properties for the imported dependency
            set_target_properties(${dependency} PROPERTIES
                IMPORTED_LOCATION "${FIREBASE_IOS_SDK_DIR}/${target_name}/${dependency}.xcframework"
            )

            # Link each dependency to the main target
            target_link_libraries(${target_name} INTERFACE ${dependency})
        endforeach()
    endfunction()

    # create_firebase_target(FirebaseAnalytics
    #     FBLPromises
    #     FirebaseCoreInternal
    #     GoogleAppMeasurementIdentitySupport
    #     FirebaseInstallations
    #     GoogleUtilities
    #     FirebaseCore
    #     GoogleAppMeasurement
    #     nanopb
    # )

    # create_firebase_target(FirebaseMessaging GoogleDataTransport)
    # target_link_libraries(FirebaseMessaging INTERFACE FirebaseAnalytics)


    # target_link_libraries(${target_messaging}
    #     PRIVATE
    #         FirebaseMessaging
    # )

endif()
