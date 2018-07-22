//
// Created by Evan Kirkiles on 7/21/18.
//

#include "window.hpp"

// Constructor for the Bot's main window
BotWindow::BotWindow(QWidget *parent) : QWidget(parent) {
    // Set the size of the window
    setFixedSize(1000, 600);
    // Also set the stylesheet for the window
    QFile File("./shopifybot/Graphics/stylesheet.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    setStyleSheet(StyleSheet);
    // Finally, set some various window attributes
    setAttribute(Qt::WA_QuitOnClose);
    setWindowTitle(tr("Shopify Bot"));
    setObjectName("main_window");

    // LAYOUTS
    // Create two vertical layouts within a horizontal main layout
    auto mainLayout = new QVBoxLayout;
    auto topLayout = new QHBoxLayout;
    auto botLayout = new QHBoxLayout;
    // Vertical layouts for the top layout
    auto leftColumn = new QVBoxLayout;
    // Combine layouts
    mainLayout->addLayout(topLayout);
    mainLayout->addStretch(4);
    mainLayout->addLayout(botLayout);
    mainLayout->addStretch(1);
    topLayout->addLayout(leftColumn);
    topLayout->addStretch(5);
    setLayout(mainLayout);

    // WIDGETS
    // Left column widgets
    QPixmap logoimg(file_paths::LOGO_FILE);
    logo = new QLabel();
    logo->setPixmap(logoimg);
    title = new QLabel("TITLE HERE", this);
    billing = new QPushButton("Billing Info", this);
    proxies = new QPushButton("Proxies", this);
    logs = new QPushButton("Logs", this);
    about = new QPushButton("About", this);
    // Add widgets to the left column
    leftColumn->addWidget(logo);
    leftColumn->addWidget(title);
    leftColumn->addWidget(billing);
    leftColumn->addWidget(proxies);
    leftColumn->addWidget(logs);
    leftColumn->addWidget(about);
}