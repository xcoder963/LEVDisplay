cmake_minimum_required(VERSION 3.5)

project(LEVScreen VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CURL_LIBRARY "-lcurl")

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS WebEngineWidgets)
find_package(Qt5 COMPONENTS Bluetooth Network)
find_package(CURL REQUIRED)
find_package(Qt5 COMPONENTS Widgets VirtualKeyboard REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        BluetoothManager.cpp
        BluetoothManager.h
        wifimanager.cpp
        wifimanager.h
        screenmanager.cpp
        screenmanager.h
        threadmanager.cpp
        threadmanager.h
        webinteractions.cpp
        webinteractions.h
)

include_directories(${CURL_INCLUDE_DIR})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(LEVScreen
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET LEVScreen APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(LEVScreen SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(LEVScreen
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(LEVScreen PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    Qt5::WebEngineWidgets Qt5::Bluetooth Qt5::Network Qt5::VirtualKeyboard ${CURL_LIBRARIES})

set_target_properties(LEVScreen PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(LEVScreen)
endif()
