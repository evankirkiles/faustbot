//
// Created by Evan Kirkiles on 7/24/18.
//

#include "titlebar.hpp"

// Builds the dark title bar
DarkTitleBar::DarkTitleBar(QWidget *parent) : parentWidg(parent){

    // Build the title bar's necessary components
    title = new QLabel(parent->windowTitle(), this);

    // Clickable buttons in top left
    closeWindow = new ClickableImage(24, 24, 2, file_paths::EXIT2_IMG, file_paths::EXIT_IMG, this);

    // Make sure the title bar always has a fixed height
    setFixedHeight(25);
    setObjectName("titlebar");

    // Uses a horizontal box layout obviously
    auto mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(5, 1, 5, 1);
    mainLayout->addWidget(closeWindow);
    mainLayout->addStretch();
    mainLayout->addWidget(title);
    mainLayout->addStretch();

    // Connect the pushbutton to the close function of the parent widget
    connect(closeWindow, SIGNAL(clicked()), parent, SLOT(close()));
}

// Override the mouse press event
void DarkTitleBar::mousePressEvent(QMouseEvent *event) {
    // When the user clicks with the left button, save the position of the click relative to window
    if (event->buttons() & Qt::LeftButton) {
        cursorPoint = event->globalPos() - parentWidg->geometry().topLeft();
        clickedOnButton = childAt(event->pos()) == closeWindow;
        event->accept();
    }
}
// Override the mouse move event
void DarkTitleBar::mouseMoveEvent(QMouseEvent *event) {
    // When the user holds the left button down, move the window around with it
    if (event->buttons() & Qt::LeftButton && !clickedOnButton) {
        parentWidg->move(event->globalPos() - cursorPoint);
        event->accept();
    }
}
