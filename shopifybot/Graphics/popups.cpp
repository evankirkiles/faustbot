//
// Created by Evan Kirkiles on 7/23/18.
//

#include "popups.hpp"

// LOG FILE DISPLAY CLASS
// Constructor that builds the window
LogFileDisplay::LogFileDisplay(const std::string& p_title, const std::string& LFlocation, QWidget *parent) :
        title(p_title), QWidget(parent) {

    // Fixed size of the window
    setFixedSize(500, 500);
    setWindowTitle("Logs");
    setObjectName("logs_window");

    // Set the stylesheet for the window
    QFile File("./shopifybot/Graphics/stylesheet.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    setStyleSheet(StyleSheet);

    // Create the window layouts
    auto mainLayout = new QVBoxLayout();
    auto topLayout = new QHBoxLayout();

    // Add the title and the refresh button to the top of the window
    titledisplay = new QLabel(title.c_str(), this);
    titledisplay->setObjectName("task_important_text");
    refreshButton = new QPushButton("REFRESH", this);
    refreshButton->setObjectName("sidebuttons");

    // Add the top layer widgets to the horizontal layout
    topLayout->addWidget(titledisplay);
    topLayout->addWidget(refreshButton);

    // Initialize the logFile and the textstream
    logFile = new QFile(LFlocation.c_str());
    logFile->open(QIODevice::ReadOnly);
    logStream = new QTextStream(logFile);
    logDisplay = new QTextBrowser();
    logDisplay->setText(logStream->readAll());

    // Add these things to the main layout
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(logDisplay);

    // Set the window's layout
    setLayout(mainLayout);
}