# Use recommended cmake version
cmake_minimum_required(VERSION 3.10)
# Set deployment target for OS X
project(shopify_bot LANGUAGES CXX)

# Set various CMake variables
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE release)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(APP_DOMAIN ./domain)
set(APP_CALC ./calc)
# Turn on Qt-related auto compiler options
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
# More runtime variables
set(prefix "${PROJECT_NAME}.app/Contents")
set(INSTALL_RUNTIME_DIR "${prefix}/MacOS")
set(INSTALL_CMAKE_DIR "${prefix}/Resources")

# Find the packages necessary
find_package(Qt5 COMPONENTS Core Widgets Concurrent Network REQUIRED)
find_package(CURL REQUIRED)

# List of sources
set(SHOPBOT_SRCS
        main.cpp
        shopifybot/WebAccess/product_scraper.cpp
        shopifybot/constants.cpp
        shopifybot/Infrastructure/tasks.cpp
        shopifybot/Graphics/window.cpp
        shopifybot/Graphics/taskwidget.cpp
        shopifybot/Graphics/popups.cpp
        shopifybot/Graphics/titlebar.cpp
        shopifybot/Graphics/sidebarpopups.cpp
        shopifybot/Graphics/customwidgets.cpp
        shopifybot/Graphics/Stylesheets/colorcustomizer.cpp)
# List of Qt Resources file
set(SHOPBOT_RCS
        shopifybot/Graphics/resources.qrc)
# List of actual resources
set(SHOPBOT_RSCS
        shopifybot/Graphics/Stylesheets/colorstylesheet.qss
        shopifybot/Graphics/Stylesheets/colorstylesheet_default.qss
        shopifybot/Infrastructure/profiles.txt
        shopifybot/Infrastructure/proxies.txt
        shopifybot/Infrastructure/ccards.txt
        shopifybot/WebAccess/Contents/CookieJar/cookies.txt
        shopifybot/WebAccess/checkout.py
        shopifybot/WebAccess/proxychecker.py)
set_property(SOURCE SHOPBOT_RSCS PROPERTY MACOSX_PACKAGE_LOCATION "Resource")

# Build the executable
add_executable(shopify_bot MACOSX_BUNDLE
        ${SHOPBOT_SRCS}
        ${SHOPBOT_RCS})

# Add Docs references
add_custom_target(Docs SOURCES README.md .gitignore LICENSE)

# --------------------------------- INCLUDES --------------------------------- #

# Include the shopifybot libraries and app domain/calc
include_directories(shopifybot)
include_directories(${APP_DOMAIN})
include_directories(${APP_CALC})

# Include cURL and link its libraries
include_directories(%{CURL_INCLUDE_DIRS})
target_link_libraries(shopify_bot ${CURL_LIBRARIES})

# Qt Related includes and linkers
include_directories(
        ${Qt5Widgets_INCLUDE_DIRS}
        ${Qt5Core_INCLUDE_DIRS}
        ${Qt5Concurrent_INCLUDE_DIRS}
        ${Qt5Network_INCLUDE_DIRS})
target_link_libraries(shopify_bot
        ${Qt5Widgets_LIBRARIES}
        ${Qt5Core_LIBRARIES}
        ${Qt5Concurrent_LIBRARIES}
        ${Qt5Network_LIBRARIES})

# ---------------------------------------------------------------------------- #

# Make macro to add in Qt5 plugins
macro(install_qt5_plugin _qt_plugin_name _qt_plugins_var _prefix)
    get_target_property(_qt_plugin_path "${_qt_plugin_name}" LOCATION)
    if (EXISTS "${_qt_plugin_path}")
        get_filename_component(_qt_plugin_file "${_qt_plugin_path}" NAME)
        get_filename_component(_qt_plugin_type "${_qt_plugin_path}" PATH)
        get_filename_component(_qt_plugin_type "${_qt_plugin_type}" NAME)
        set(_qt_plugin_dest "${_prefix}/PlugIns/${_qt_plugin_type}")
        install(FILES "${_qt_plugin_path}"
                DESTINATION "${_qt_plugin_dest}")
        set(${_qt_plugins_var}
                "${${_qt_plugins_var}};\$ENV{DEST_DIR}\${CMAKE_INSTALL_PREFIX}/${_qt_plugin_dest}/${_qt_plugin_file}")
    else()
        message(FATAL_ERROR "Qt plugin ${_qt_plugin_name} not found")
    endif()
endmacro()

# Install Cocoa integration as it is a plugin required by QtGui which is required by QWidgets
install_qt5_plugin("Qt5::QCocoaIntegrationPlugin" QT_PLUGINS ${prefix})
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/qt.conf"
        "[Paths]\nPlugins = ${_qt_plugin_dir}\n")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/qt.conf"
        DESTINATION "${INSTALL_CMAKE_DIR}")

# Destination paths below are relative to ${CMAKE_INSTALL_PREFIX}
install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION . COMPONENT Runtime
        RUNTIME DESTINATION ${INSTALL_RUNTIME_DIR} COMPONENT Runtime)

# Mac specific extension .app
set(APPS "\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}.app")

# Directories to look for dependencies
set(DIRS "${CMAKE_BINARY_DIR}")

# Path used for searching by FIND_XXX(), with appropriate suffixes added
if (CMAKE_PREFIX_PATH)
    foreach(dir ${CMAKE_PREFIX_PATH})
        list(APPEND DIRS "${dir}/bin" "${dir}/lib")
    endforeach()
endif()

# Append Qt's lib folder which is two levels above Qt5Widgets_DIR
list(APPEND DIRS "${Qt5Widgets_DIR}/../..")
# Also install required system libraries
include(InstallRequiredSystemLibraries)

# Perform some logging
message(STATUS "APPS: ${APPS}")
message(STATUS "QT_PLUGINS: ${QT_PLUGINS}")
message(STATUS "DIRS: ${DIRS}")

# Finally perform bundling
install(CODE "include(BundleUtilities)
    fixup_bundle(\"${APPS}\" \"${QT_PLUGINS}\" \"${DIRS}\")")

# Move the resources into the binary directory
configure_file(shopifybot/Graphics/Stylesheets/colorstylesheet.qss ${CMAKE_CURRENT_BINARY_DIR}/Resources/Stylesheets/colorstylesheet.qss COPYONLY)
configure_file(shopifybot/Graphics/Stylesheets/colorstylesheet_default.qss ${CMAKE_CURRENT_BINARY_DIR}/Resources/Stylesheets/colorstylesheet_default.qss COPYONLY)
configure_file(shopifybot/Infrastructure/profiles.txt ${CMAKE_CURRENT_BINARY_DIR}/Resources/Cache/profiles.txt COPYONLY)
configure_file(shopifybot/Infrastructure/proxies.txt ${CMAKE_CURRENT_BINARY_DIR}/Resources/Cache/proxies.txt COPYONLY)
configure_file(shopifybot/Infrastructure/ccards.txt ${CMAKE_CURRENT_BINARY_DIR}/Resources/Cache/ccards.txt COPYONLY)
configure_file(shopifybot/WebAccess/Contents/CookieJar/cookies.txt ${CMAKE_CURRENT_BINARY_DIR}/Resources/Contents/CookieJar/cookies.txt COPYONLY)
configure_file(shopifybot/WebAccess/checkout.py ${CMAKE_CURRENT_BINARY_DIR}/Resources/checkout.py COPYONLY)
configure_file(shopifybot/WebAccess/proxychecker.py ${CMAKE_CURRENT_BINARY_DIR}/Resources/proxychecker.py COPYONLY)

# Pack everything up
set(CPACK_GENERATOR "DRAGNDROP")
include(CPack)