project(shopifybot)

set(HEADER_FILES
        constants.hpp
        Infrastructure/interface.hpp
        Infrastructure/tasks.hpp
        WebAccess/product_scraper.hpp
        Graphics/window.hpp
        Graphics/taskwidget.hpp)

set(SOURCE_FILES
        constants.cpp
        Infrastructure/interface.cpp
        Infrastructure/tasks.cpp
        WebAccess/product_scraper.cpp
        Graphics/window.cpp
        Graphics/taskwidget.cpp)

add_library(shopifybot STATIC ${SOURCE_FILES} ${HEADER_FILES})