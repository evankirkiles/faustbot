//
// Created by Evan Kirkiles on 8/1/18.
//

#include "sidebarpopups.hpp"

// PROFILES DISPLAY
// Constructor that builds the profiles display
ProfilesDisplay::ProfilesDisplay(QWidget *parent) : QWidget(parent) {

    // Set window proprties
    setFixedSize(500, 500);
    setWindowTitle("Profiles");
    setWindowFlags(Qt::FramelessWindowHint);
    setFocusPolicy(Qt::ClickFocus);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_TranslucentBackground);

    // Create the title bar
    dtb = new DarkTitleBar(this, true);

    // Set the stylesheet for the window
    QFile File("./shopifybot/Graphics/stylesheet.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    setStyleSheet(StyleSheet);

    // Create external qframe and layouts for dtb
    auto externLayout = new QVBoxLayout();
    externLayout->setContentsMargins(0, 0, 0, 0);
    auto bg = new QFrame(this);
    auto bgLayout = new QVBoxLayout();
    bgLayout->setContentsMargins(0, 0, 0, 0);
    bg->setObjectName("main_window");
    bg->setLayout(bgLayout);
    bgLayout->addWidget(dtb);
    externLayout->addWidget(bg);

    // Create normal layouts
    auto mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(11, 3, 11, 11);
    auto leftColumn = new QVBoxLayout();
    auto smallButtonRow = new QHBoxLayout();
    auto rightColumn = new QVBoxLayout();
    auto titleRow = new QHBoxLayout();

    // Create the widgets and add them to their respective layouts
    leftColumn->addLayout(smallButtonRow);
    profilesListView = new QListView(this);
    profilesListView->setObjectName("profileslistview");
    leftColumn->addWidget(profilesListView);
    titleLabel = new QLabel("Title: ", this);
    titleLabel->setObjectName("task_important_text");
    editTitle = new QLineEdit(this);
    editTitle->setObjectName("task_title_lineedit");
    titleRow->addWidget(titleLabel);
    titleRow->addWidget(editTitle);

    // Finish adding all the layouts
    mainLayout->addLayout(leftColumn);
    mainLayout->addLayout(rightColumn);
    rightColumn->addLayout(titleRow);
    bgLayout->addLayout(mainLayout);

    // Set the layout finally
    setLayout(externLayout);
}

// Override the column for the profiles display
void ProfilesDisplay::closeEvent(QCloseEvent* event) {
    emit closed();
    QWidget::closeEvent(event);
}