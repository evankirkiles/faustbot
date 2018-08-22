#include <iostream>

#ifndef QApplication
#include <QApplication>
#endif

#include "Graphics/Stylesheets/colorcustomizer.hpp"

#include "Infrastructure/tasks.hpp"
#include "Graphics/window.hpp"

int main(int argc, char **argv) {

    // Test the color customizer
    ColorCustomizer cc = ColorCustomizer();
    cc.resetAllValues();

    // Builds the QApplication
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    app.setAttribute(Qt::AA_EnableHighDpiScaling);

    // Render the Qt bot window
    BotWindow window = BotWindow();
    window.show();
    window.raise();

    // Run the application
    app.exec();

    return 0;
}