cmake_minimum_required(VERSION 3.10)
project(runshopifytests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(APP_DOMAIN ./domain)
set(APP_CALC ./calc)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(runshopifytests
        WebHandler_UTESTS.cpp)

include_directories(shopifybot_tests/lib/googletest/include shopifybot_tests/lib/googletest)
include_directories(shopifybot)

find_package(CURL REQUIRED)
IF (CURL_FOUND)
    include_directories(%{CURL_INCLUDE_DIRS})
    target_link_libraries(runshopifytests ${CURL_LIBRARIES})
endif()
find_package(Qt5Widgets REQUIRED)
IF (Qt5Widgets_FOUND)
    include_directories(${Qt5Widgets_INCLUDE_DIRS})
    target_link_libraries(runshopifytests ${Qt5Widgets_LIBRARIES})
endif()
find_package(Qt5Core REQUIRED)
IF (Qt5Core_FOUND)
    include_directories(${Qt5Core_INCLUDE_DIRS})
    target_link_libraries(runshopifytests ${Qt5Core_LIBRARIES})
endif()

target_link_libraries(runshopifytests gtest gtest_main)
target_link_libraries(runshopifytests shopifybot)

include_directories(${APP_DOMAIN})
include_directories(${APP_CALC})