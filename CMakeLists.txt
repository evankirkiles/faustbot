cmake_minimum_required(VERSION 3.10)
project(shopify_bot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(APP_DOMAIN ./domain)
set(APP_CALC ./calc)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(shopify_bot
        main.cpp
        shopifybot/WebAccess/product_scraper.cpp
        shopifybot/constants.cpp
        shopifybot/Infrastructure/interface.cpp
        shopifybot/Infrastructure/tasks.cpp)

include_directories(shopifybot)
include_directories(shopifybot_tests/lib/googletest/include shopifybot_tests/lib/googletest)

add_subdirectory(shopifybot)
add_subdirectory(shopifybot_tests)

target_link_libraries(shopifybot)

find_package(CURL REQUIRED)
IF (CURL_FOUND)
    include_directories(%{CURL_INCLUDE_DIRS})
    target_link_libraries(shopify_bot ${CURL_LIBRARIES})
endif()

include_directories(${APP_DOMAIN})
include_directories(${APP_CALC})