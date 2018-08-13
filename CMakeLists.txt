cmake_minimum_required(VERSION 3.10)
project(shopify_bot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH /Users/evankirkiles/Qt/5.11.1/clang_64)
set(APP_DOMAIN ./domain)
set(APP_CALC ./calc)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(shopify_bot
        main.cpp
        shopifybot/WebAccess/product_scraper.cpp
        shopifybot/constants.cpp
        shopifybot/Infrastructure/tasks.cpp
        shopifybot/Graphics/window.cpp
        shopifybot/Graphics/taskwidget.cpp
        shopifybot/Graphics/popups.cpp
        shopifybot/Graphics/titlebar.cpp
        shopifybot/Graphics/sidebarpopups.cpp
        shopifybot/Graphics/customwidgets.cpp)

include_directories(shopifybot)
include_directories(shopifybot_tests/lib/googletest/include shopifybot_tests/lib/googletest)

add_subdirectory(shopifybot)
add_subdirectory(shopifybot_tests)

find_package(CURL REQUIRED)
IF (CURL_FOUND)
    include_directories(%{CURL_INCLUDE_DIRS})
    target_link_libraries(shopify_bot ${CURL_LIBRARIES})
endif()
find_package(Qt5Widgets REQUIRED)
IF (Qt5Widgets_FOUND)
    include_directories(${Qt5Widgets_INCLUDE_DIRS})
    target_link_libraries(shopify_bot ${Qt5Widgets_LIBRARIES})
endif()
find_package(Qt5Core REQUIRED)
IF (Qt5Core_FOUND)
    include_directories(${Qt5Core_INCLUDE_DIRS})
    target_link_libraries(shopify_bot ${Qt5Core_LIBRARIES})
endif()
find_package(Qt5Concurrent REQUIRED)
IF (Qt5Concurrent_FOUND)
    include_directories(${Qt5Concurrent_INCLUDE_DIRS})
    target_link_libraries(shopify_bot ${Qt5Concurrent_LIBRARIES})
endif()

include_directories(${APP_DOMAIN})
include_directories(${APP_CALC})