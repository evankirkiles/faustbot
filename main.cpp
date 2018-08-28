#include <iostream>

#ifndef QApplication
#include <QApplication>
#endif

#include "WebAccess/checkout.hpp"

#include "Graphics/Stylesheets/colorcustomizer.hpp"

#include "Infrastructure/tasks.hpp"
#include "Graphics/window.hpp"

int main(int argc, char **argv) {

    // Builds the QApplication
    QApplication app(argc, argv);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // Render the Qt bot window
    BotWindow window = BotWindow();
    window.show();
    window.raise();

    Checkout checker("https://kith.com/cart/2600793800711:1", "kith.com", "/../Logs/checking.txt", "Random", "Random", "1");
    checker.run();

    // Run the application
    app.exec();


    return 0;
}