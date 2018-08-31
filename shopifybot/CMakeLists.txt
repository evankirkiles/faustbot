project(shopifybot)

set(HEADER_FILES
        constants.hpp
        Infrastructure/tasks.hpp
        WebAccess/product_scraper.hpp
        WebAccess/checkout.hpp
        WebAccess/proxychecker.hpp
        Graphics/window.hpp
        Graphics/taskwidget.hpp
        Graphics/popups.hpp
        Graphics/titlebar.hpp
        Graphics/sidebarpopups.hpp
        Graphics/customwidgets.hpp
        Graphics/Stylesheets/colorcustomizer.hpp)

set(SOURCE_FILES
        constants.cpp
        Infrastructure/tasks.cpp
        WebAccess/product_scraper.cpp
        WebAccess/checkout.cpp
        WebAccess/proxychecker.cpp
        Graphics/window.cpp
        Graphics/taskwidget.cpp
        Graphics/popups.cpp
        Graphics/titlebar.cpp
        Graphics/sidebarpopups.cpp
        Graphics/customwidgets.cpp
        Graphics/Stylesheets/colorcustomizer.cpp)

add_library(shopifybot STATIC ${SOURCE_FILES} ${HEADER_FILES})