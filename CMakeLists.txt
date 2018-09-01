# Use recommended cmake version
cmake_minimum_required(VERSION 3.10)
# Set deployment target for OS X
project(Faust LANGUAGES CXX)

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
        shopifybot/WebAccess/checkout.cpp
        shopifybot/WebAccess/proxychecker.cpp
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
        shopifybot/logo.icns)
set(SHOPBOT_UINFO
        shopifybot/Infrastructure/profiles.txt
        shopifybot/Infrastructure/proxies.txt
        shopifybot/Infrastructure/ccards.txt)
set(SHOPBOT_LOGS
        shopifybot/WebAccess/pylogs.txt)
set(SHOPBOT_COOKIE
        shopifybot/WebAccess/Contents/CookieJar/cookies.txt)
set(SHOPBOT_STYLESHEETS
        shopifybot/Graphics/Stylesheets/colorstylesheet.qss
        shopifybot/Graphics/Stylesheets/colorstylesheet_default.qss)
# Make sure the resources get put in the Contents/Resources folder
set_source_files_properties(${SHOPBOT_RSCS} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
set_source_files_properties(${SHOPBOT_LOGS} PROPERTIES MACOSX_PACKAGE_LOCATION "Logs")
set_source_files_properties(${SHOPBOT_UINFO} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/Vault")
set_source_files_properties(${SHOPBOT_COOKIE} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/Contents/CookieJar")
set_source_files_properties(${SHOPBOT_STYLESHEETS} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/Stylesheets")

# Set the Mac icon
set(MACOSX_BUNDLE_ICON_FILE logo.icns)

# Build the executable
add_executable(Faust MACOSX_BUNDLE
        ${SHOPBOT_SRCS}
        ${SHOPBOT_RCS}
        ${SHOPBOT_RSCS}
        ${SHOPBOT_UINFO}
        ${SHOPBOT_LOGS}
        ${SHOPBOT_COOKIE}
        ${SHOPBOT_STYLESHEETS})

# Add Docs references
add_custom_target(Docs SOURCES README.md .gitignore LICENSE)

# Set some source properties
set_source_files_properties(Faust PROPERTIES OUTPUT_NAME "Faust")

# --------------------------------- INCLUDES --------------------------------- #

# Include the shopifybot libraries and app domain/calc
include_directories(shopifybot)
include_directories(${APP_DOMAIN})
include_directories(${APP_CALC})

# Include cURL and link its libraries
include_directories(%{CURL_INCLUDE_DIRS})
target_link_libraries(Faust ${CURL_LIBRARIES})

# Qt Related includes and linkers
include_directories(
        ${Qt5Widgets_INCLUDE_DIRS}
        ${Qt5Core_INCLUDE_DIRS}
        ${Qt5Concurrent_INCLUDE_DIRS}
        ${Qt5Network_INCLUDE_DIRS})
target_link_libraries(Faust
        ${Qt5Widgets_LIBRARIES}
        ${Qt5Core_LIBRARIES}
        ${Qt5Concurrent_LIBRARIES}
        ${Qt5Network_LIBRARIES})

# ---------------------------------------------------------------------------- #

# Install the plugins for each qt module
set(REQ_QT_PLUGINS
        ${Qt5Core_PLUGINS}
        ${Qt5Widgets_PLUGINS}
        ${Qt5Network_PLUGINS}
        ${Qt5Gui_PLUGINS}
        ${Qt5Concurrent_PLUGINS})
foreach(plugin ${REQ_QT_PLUGINS})
    get_target_property(_qt_plugin_path ${plugin} LOCATION)
    get_filename_component(_qt_plugin_file "${_qt_plugin_path}" NAME)
    get_filename_component(_qt_plugin_type "${_qt_plugin_path}" PATH)
    get_filename_component(_qt_plugin_type "${_qt_plugin_type}" NAME)
    set(_qt_plugin_dest "${prefix}/PlugIns/${_qt_plugin_type}")
    install(FILES "${_qt_plugin_path}"
        DESTINATION "${_qt_plugin_dest}")
    set(QT_PLUGINS
            "${QT_PLUGINS};\$ENV{DEST_DIR}\${CMAKE_INSTALL_PREFIX}/${_qt_plugin_dest}/${_qt_plugin_file}")
endforeach()

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
install(CODE "
    file(GLOB_RECURSE QT_PLUGINS
    \"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/plugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
    include(BundleUtilities)
    fixup_bundle(\"${APPS}\" \"${QT_PLUGINS}\" \"${DIRS}\")")

# Move the resources into the binary directory
configure_file(shopifybot/WebAccess/proxychecker.py ${CMAKE_CURRENT_BINARY_DIR}/Resources/proxychecker.py COPYONLY)

# Pack everything up
set(CPACK_GENERATOR "DRAGNDROP")
include(CPack)