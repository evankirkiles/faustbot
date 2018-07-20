project(shopifybot)

set(HEADER_FILES
        constants.hpp
        Infrastructure/interface.hpp
        Infrastructure/tasks.hpp
        WebAccess/product_scraper.hpp)

set(SOURCE_FILES
        constants.cpp
        Infrastructure/interface.cpp
        Infrastructure/tasks.cpp
        WebAccess/product_scraper.cpp)

add_library(shopifybot STATIC ${SOURCE_FILES} ${HEADER_FILES})