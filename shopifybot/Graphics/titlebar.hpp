//
// Created by Evan Kirkiles on 7/24/18.
//

#ifndef SHOPIFY_BOT_TITLEBAR_HPP
#define SHOPIFY_BOT_TITLEBAR_HPP

#ifndef QWidget
#include <QWidget>
#endif
#ifndef QMouseEvent
#include <QMouseEvent>
#endif
#ifndef QLabel
#include <QLabel>
#endif
#ifndef QPushButton
#include <QPushButton>
#endif
#ifndef QHBoxLayout
#include <QHBoxLayout>
#endif

#include <iostream>

#include "constants.hpp"

// Clickable image without checkable ability
class ClickableImage : public QLabel {
Q_OBJECT
public:
    // Default constructor and destructors
    ClickableImage(int width, int height, double ratio, const char* pressedImg, const char* releasedImg, QWidget *parent = 0) :
            pressed(QPixmap::fromImage(QImage(pressedImg).scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation))),
            released(QPixmap::fromImage(QImage(releasedImg).scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation))),
            QLabel(parent) {
        pressed.setDevicePixelRatio(ratio);
        released.setDevicePixelRatio(ratio);

        // Set the default image to be released
        setPixmap(released);
        setFixedSize(width/ratio, height/ratio);
    }

// Emit the clicked() signal when the QLabel is clicked on
signals:
    void clicked();
protected:
    // Save both states of the image (clicked and not clicked)
    QPixmap pressed;
    QPixmap released;

    void mousePressEvent(QMouseEvent *event) override {
        // Change to the presed image
        setPixmap(pressed);
        QWidget::mousePressEvent(event);
    }
    void mouseReleaseEvent(QMouseEvent *event) override {
        // Make the label go back to its initial image
        setPixmap(released);
        emit clicked();
        QWidget::mouseReleaseEvent(event);
    }
};

// Class containing the custom title bar for the windows, allowing for good looks on both Mac and Windows
class DarkTitleBar : public QFrame {
    Q_OBJECT
private:
    // References to the parent widget and th eposition of the cursor
    QWidget* parentWidg;
    QPoint cursorPoint;

    // Makes sure not clicking on an exit button when moving
    bool clickedOnButton = false;

    // The actual title of the window as well as a pushbutton to close it
    QLabel* title;

    // Buttons
    ClickableImage* closeWindow;

public:
    // Constructor that builds the title bar
    DarkTitleBar(QWidget* parent, bool moreInfo = false);
    // Optional moreinfo button
    ClickableImage* moreInfoButton;
protected:
    // Override mousepressevent and mousemoveevent
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif //SHOPIFY_BOT_TITLEBAR_HPP
