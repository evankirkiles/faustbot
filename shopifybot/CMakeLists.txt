project(shopifybot)

set(HEADER_FILES
        constants.hpp
        Infrastructure/tasks.hpp
        WebAccess/product_scraper.hpp
        Graphics/window.hpp
        Graphics/taskwidget.hpp
        Graphics/popups.hpp)

set(SOURCE_FILES
        constants.cpp
        Infrastructure/tasks.cpp
        WebAccess/product_scraper.cpp
        Graphics/window.cpp
        Graphics/taskwidget.cpp
        Graphics/popups.cpp)

add_library(shopifybot STATIC ${SOURCE_FILES} ${HEADER_FILES})