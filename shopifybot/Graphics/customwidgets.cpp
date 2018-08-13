//
// Created by Evan Kirkiles on 8/13/18.
//

#include "customwidgets.hpp"

// Constructor for the clickable checkable image
ClickableCheckableImage::ClickableCheckableImage(int p_width, int p_height, double p_ratio, const char* pressedImg, const char* releasedImg,
                                                 const char* pressedImg2, const char* releasedImg2, const char* disabledImg,
                                                 QWidget *parent) :
        width(p_width), height(p_height), ratio(p_ratio),
        pressed(QPixmap::fromImage(QImage(pressedImg).scaled(p_width, p_height, Qt::KeepAspectRatio, Qt::SmoothTransformation))),
        released(QPixmap::fromImage(QImage(releasedImg).scaled(p_width, p_height, Qt::KeepAspectRatio, Qt::SmoothTransformation))),
        pressed2(QPixmap::fromImage(QImage(pressedImg2).scaled(p_width, p_height, Qt::KeepAspectRatio, Qt::SmoothTransformation))),
        released2(QPixmap::fromImage(QImage(releasedImg2).scaled(p_width, p_height, Qt::KeepAspectRatio, Qt::SmoothTransformation))),
        disabled(QPixmap::fromImage(QImage(disabledImg).scaled(p_width, p_height, Qt::KeepAspectRatio, Qt::SmoothTransformation))),
        QLabel(parent) {
    pressed.setDevicePixelRatio(p_ratio);
    pressed2.setDevicePixelRatio(p_ratio);
    released.setDevicePixelRatio(p_ratio);
    released2.setDevicePixelRatio(p_ratio);
    disabled.setDevicePixelRatio(p_ratio);

    // Set the default image to be the released, not checked option
    setPixmap(released);
    setMaximumWidth(width/ratio);
}

// Changes the checked image
void ClickableCheckableImage::changeCheckedImg(const char* pressedImg, const char* releasedImg) {
    pressed = QPixmap::fromImage(QImage(pressedImg).scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    released = QPixmap::fromImage(QImage(releasedImg).scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    pressed.setDevicePixelRatio(ratio);
    released.setDevicePixelRatio(ratio);
}

// Play / stop specific functions for the play task button
void ClickableCheckableImage::clickedPlay() { setPixmap(released2); isChecked = true; }
void ClickableCheckableImage::clickedStop() { setPixmap(released); isChecked = false; }

// SLOTS
void ClickableCheckableImage::enable() { setPixmap(released); enabled = true; };
void ClickableCheckableImage::disable() { setPixmap(disabled); enabled = false; };