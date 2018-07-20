set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(APP_DOMAIN ./domain)
set(APP_CALC ./calc)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include_directories(shopifybot_tests/lib/googletest/include shopifybot_tests/lib/googletest)

add_executable(runshopifytests
        WebHandler_UTESTS.cpp)

find_package(CURL REQUIRED)
IF (CURL_FOUND)
    include_directories(%{CURL_INCLUDE_DIRS})
    target_link_libraries(runshopifytests ${CURL_LIBRARIES})
endif()

target_link_libraries(runshopifytests gtest gtest_main)
target_link_libraries(runshopifytests shopifybot)