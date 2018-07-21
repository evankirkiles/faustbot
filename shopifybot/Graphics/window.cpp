//
// Created by Evan Kirkiles on 7/21/18.
//

#include "window.hpp"

// Constructor for the Bot's main window
BotWindow::BotWindow(QWidget *parent) : QWidget(parent) {
    // Set the size of the window
    setFixedSize(500, 500);
    // Also set the stylesheet for the window
    QFile File("./shopifybot/Graphics/stylesheet.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    setStyleSheet(StyleSheet);
    // Finally, set some various window attributes
    setAttribute(Qt::WA_QuitOnClose);
    setWindowTitle(tr("Shopify Bot"));
    setObjectName("main_window");
}