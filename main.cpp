#include <iostream>

#ifndef QApplication
#include <QApplication>
#endif

#include "Infrastructure/tasks.hpp"
#include "Graphics/window.hpp"

int main(int argc, char **argv) {

    // Create two test tanks and run them
    // Task task1("Kith Task", supported_sites::KITH, "1", "/collections/footwear", {"Asics"}, {"White"}, "9", 3, 3);
    // Task task2("Bodega Task", supported_sites::BODEGA, "1", "/collections/footwear", {"Air Max"}, {"Cargo"}, "9", 1, 3);

    // std::thread t1(&Task::run, task1);
    // std::thread t2(&Task::run, task2);

    // t1.join();
    // t2.join();

    // Builds the QApplication
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DisableHighDpiScaling);
    // app.setAttribute(Qt::AA_UseHighDpiPixmaps);

    // Render the Qt bot window
    BotWindow window = BotWindow();
    // window.addTask("Bodega Task", supported_sites::BODEGA, "1", "/collections/footwear", {"Air Max"}, {"Cargo", "White"}, "9", 1, 3);

    window.show();
    window.raise();

    // Run the application
    app.exec();

    return 0;
}