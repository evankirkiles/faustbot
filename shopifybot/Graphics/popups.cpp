//
// Created by Evan Kirkiles on 7/23/18.
//

#include "popups.hpp"

// LOG FILE DISPLAY CLASS
// Constructor that builds the window
LogFileDisplay::LogFileDisplay(const std::string& p_title, const std::string& LFlocation, QWidget *parent) :
        title(p_title), QWidget(parent) {

    // Fixed size of the window
    setFixedSize(500, 300);
    setWindowTitle("Logs");
    setObjectName("logs_window");
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_QuitOnClose, false);

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
    if (logFile->exists()) {
        logStream = new QTextStream(logFile);
        logDisplay = new QTextBrowser();
        logDisplay->setText(logStream->readAll());
    } else {
        logDisplay = new QTextBrowser();
        logDisplay->setText(QString("Log file not yet generated."));
    }

    logDisplay->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    logDisplay->setObjectName("logs_text");

    // Add these things to the main layout
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(logDisplay);

    // Set the window's layout
    setLayout(mainLayout);

    // Connect the refresh button to its slot
    connect(refreshButton, SIGNAL(clicked()), this, SLOT(refresh()));
}

// Refresh slot which reloads the filestream into the textbrowser
void LogFileDisplay::refresh() {
    logFile->close();
    logFile->open(QIODevice::ReadOnly);
    if (logFile->exists()) {
        logStream = new QTextStream(logFile);
        logDisplay->setText(logStream->readAll());
    } else {
        logDisplay->setText(QString("Log file not yet generated."));
    }
}

// Custom close event function that just emits a signal signifying it has closed
void LogFileDisplay::closeEvent(QCloseEvent *event) {
    // Emit the closed signal and then proceed to cleanup
    emit closed();
    QWidget::closeEvent(event);
}

// ADD TASK DISPLAY CLASS
// Constructor that builds the window for adding a task
AddTaskDisplay::AddTaskDisplay(QWidget *parent) : QWidget(parent) {

    // Set window properties
    setFixedSize(500, 500);
    setObjectName("newtaskwindow");
    setWindowTitle("New Task");
    setFocusPolicy(Qt::ClickFocus);
    setAttribute(Qt::WA_QuitOnClose, false);

    // Set the stylesheet for the window
    QFile File("./shopifybot/Graphics/stylesheet.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    setStyleSheet(StyleSheet);

    // Create layouts
    auto mainLayout = new QVBoxLayout();
    // Individual horizontal row layouts
    auto topTitleLayout = new QHBoxLayout();

    // Create the widgets
    // TITLE ROW
    titleLabel = new QLabel("Title: ", this);
    titleLabel->setObjectName("task_important_text");
    title = new QTextEdit(this);
    title->setObjectName("task_title_textedit");
    topTitleLayout->addWidget(titleLabel);
    topTitleLayout->addWidget(title);
    mainLayout->addLayout(topTitleLayout);

    // Set the layout
    setLayout(mainLayout);
}


// Custom close event function that just emits a signal signifying it has closed
void AddTaskDisplay::closeEvent(QCloseEvent *event) {
    // Emit the closed signal and then proceed to cleanup
    emit closed();
    QWidget::closeEvent(event);
}