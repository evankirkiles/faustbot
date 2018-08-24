#include <iostream>

#ifndef QApplication
#include <QApplication>
#endif

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

    // Run the application
    app.exec();

    return 0;
}