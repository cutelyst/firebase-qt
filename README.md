# firebase-qt
Qt Wrapper for Firebase C++ API (CMake only)

Supports:
 * Messaging for Push Notifications
 * Phone Authentication

> It's important to note that the Firebase C++ SDK is also an wrapper around Firebase iOS and Android libraries.

It's also important to use the right Firebase SDK with the right version of iOS and or Android libraries, such as
Firebase C++ SDK 13.3.0 with Firebase iOS SDK 12.6.0.

Since these libraries take a lot of disk space it's recommended to save them under a directory that is not in
your source tree such as:

```
code/
    my_app/
    firebase_sdk/
        firebase_cpp_sdk_13.3.0/
        firebase_ios_sdk_12.6.0/
```

Then set in your CMake:
```
set(FIREBASE_CPP_SDK_DIR "${CMAKE_SOURCE_DIR}/../firebase_sdk/firebase_cpp_sdk_13.3.0" CACHE STRING "" FORCE)
set(FIREBASE_IOS_SDK_DIR "${CMAKE_SOURCE_DIR}/../firebase_sdk/firebase_ios_12.6.0" CACHE STRING "" FORCE)
```

Download them from:
- https://github.com/firebase/firebase-cpp-sdk/releases/tag/v13.3.0 (click "Prebuilt versions of the libraries are available for download **HERE**")
- https://github.com/firebase/firebase-ios-sdk/releases/tag/12.6.0 (Firebase.zip)


## Example of usage

Google Firebase SDK use futures that runs on different threads, so in order to avoid a crash due updating a Qt GUI from the wrong thread we internally use Qt:: QueuedConnection.

Initialize:

    auto firebase = new FirebaseQtApp(this);
    auto messaging = new FirebaseQtMessaging(firebase);
    connect(messaging, &FirebaseQtMessaging::tokenReceived, this, &Central::firebaseUpdateMessagingToken);
    connect(messaging, &FirebaseQtMessaging::messageReceived, this, &Central::firebaseOnMessage);

    m_firebaseAuth = new FirebaseQtAuth(firebase);
    connect(m_firebaseAuth, &FirebaseQtAuth::signInToken, this, &Central::firebaseUserToken);
    connect(m_firebaseAuth, &FirebaseQtAuth::signInError, this, &Central::firebaseUserTokenError);

    m_firebaseAuthPhone = new FirebaseQtAuthPhone(m_firebaseAuth);
    connect(m_firebaseAuthPhone, &FirebaseQtAuthPhone::codeSent, this, &Central::firebaseAuthCodeSent);
    connect(m_firebaseAuthPhone, &FirebaseQtAuthPhone::verificationFailed, this, &Central::firebaseAuthFailed);
    connect(m_firebaseAuthPhone, &FirebaseQtAuthPhone::verificationCompleted, this, &Central::firebaseAuthCompleted);

    firebase->initialize();

Ask for a phone number authentication:

    m_firebaseAuthPhone->verifyPhoneNumber("+55999999999");

Link with what you need:

```
target_link_libraries(${app_target}
    PRIVATE
        FirebaseQt::Messaging
        FirebaseQt::Auth
        FirebaseQt::App
)
```

## Compiling Android

This version only supports CMake, Firebase 13.3 has one link issue so for now we use 12.2.0
Firebase-qt can download the firebase_cpp-sdk for you but on gradle part you will need to
point to where it should be so we are not _that_ automated yet.

    if(ANDROID)
        set(FIREBASE_CPP_SDK_DIR "..path/to/firebase_cpp_sdk_13.3.0/")
        include(FetchContent)
        FetchContent_Declare(
            firebase_qt
            GIT_REPOSITORY https://github.com/cutelyst/firebase-qt.git
            GIT_TAG        some_sha
            EXCLUDE_FROM_ALL
        )
        FetchContent_MakeAvailable(firebase_qt)
    endif()


Now an important step is to update your gradle in you project/android director, the one that
QtCreator created it's template, this is essential else you will run into DEX issues,
this should be run in project_sources/android directory not on your build director.
Also you must comment all Qt variables else it will fail:

    ./gradlew wrapper --gradle-version 8.7

Next gradle.properties where you also put your firebase_cpp-sdk path:

    systemProp.firebase_cpp_sdk.dir=..path/to/firebase_cpp_sdk_13.3.0/

Last build.gradle:

    buildscript {
        repositories {
            google()
            mavenCentral()
        }

        dependencies {
            # Notice the 8.6 version here, not sure why but seems we always need to be one version less
            # than what we updated
            classpath 'com.android.tools.build:gradle:8.6.0'
            classpath 'com.google.gms:google-services:4.4.2'  // Google Services plugin

            # the lines below won't need to be changed later as the path is read from gradle.properties
            def firebase_cpp_sdk_dir = System.getProperty('firebase_cpp_sdk.dir')

            gradle.ext.firebase_cpp_sdk_dir = "$firebase_cpp_sdk_dir"
            apply from: "$firebase_cpp_sdk_dir/Android/firebase_dependencies.gradle"
        }
    }

    ...
    apply plugin: 'com.android.application'
    apply plugin: 'com.google.gms.google-services'  // Google Services plugin
    ...

    # This can be placed at the end of the file
    # You must put here all modules you want from firebase
    firebaseCpp.dependencies {
        analytics
        messaging
        auth
    }

### AndroidManifest.xml

With the above code you will get Firebase working, which can give you a valid device token to send notifications.
However in order to receive them when the app is running you must add what firebase-cpp-sdk/AndroidManafest.xml
has to your own AndroidManifest.xml.

To receive notifications when the app is not running or hidden it requires Notifications permissions that
should by programatically asked: `android.permission.POST_NOTIFICATIONS`.

## Compiling iOS

I'm trying to improve things on iOS, but so far it's in a better shape than qmake was.

`app_target` is the name of your application target, this function is present in the FirebaseQt CMake files
but I did not manage to get the app building if I run it there, so for now just add it

```
if(IOS)
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

    create_firebase_target(FirebaseAnalytics
        FBLPromises
        FirebaseCoreInternal
        GoogleAppMeasurementIdentitySupport
        FirebaseInstallations
        GoogleUtilities
        FirebaseCore
        GoogleAppMeasurement
        nanopb
    )

    create_firebase_target(FirebaseMessaging GoogleDataTransport)
    target_link_libraries(FirebaseMessaging INTERFACE FirebaseAnalytics)

    target_link_libraries(${app_target}
        PRIVATE
            FirebaseMessaging
    )

    target_link_directories(${app_target} PRIVATE
    "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/swift/${CMAKE_OSX_SYSROOT}")

    set(asset_catalog_path
        ios/Assets.xcassets
        ios/GoogleService-Info.plist
    )
    target_sources(${app_target} PRIVATE "${asset_catalog_path}")
    set_source_files_properties(
        ${asset_catalog_path}
        PROPERTIES MACOSX_PACKAGE_LOCATION Resources
    )

    set_target_properties(${app_target}
        PROPERTIES
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/ios/Info.plist"
            XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${CMAKE_CURRENT_SOURCE_DIR}/ios/${app_target}.entitlements"
            XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME AppIcon
            XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1"
    )
endif()
```

Where `Assets.xcassets` is not important for Firebase but you will likely have it due your App's icons.

Info.plist should have your App's information and be sure to add:
```
<key>FirebaseAppDelegateProxyEnabled</key>
<true/>
```

`${app_target}.entitlements` should have the following content:
```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>aps-environment</key>
    <string>development</string>
</dict>
</plist>
```
