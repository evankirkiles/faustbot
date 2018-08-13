//
// Created by Evan Kirkiles on 8/13/18.
//

#ifndef SHOPIFY_BOT_CUSTOMWIDGETS_HPP
#define SHOPIFY_BOT_CUSTOMWIDGETS_HPP

#ifndef QLabel
#include <QLabel>
#endif
#ifndef QWidget
#include <QWidget>
#endif
#ifndef QImage
#include <QImage>
#endif
#ifndef QPixmap
#include <QPixmap>
#endif
#ifndef QPainter
#include <QPainter>
#endif
#ifndef QString
#include <QString>
#endif

// Custom clickable label to allow for images to be pressed and checked
class ClickableCheckableImage : public QLabel {
    Q_OBJECT
public:
    // Default constructor and destructors
    ClickableCheckableImage(int p_width, int p_height, double p_ratio, const char* pressedImg, const char* releasedImg,
                            const char* pressedImg2, const char* releasedImg2, const char* disabledImg,
                            QWidget *parent = 0);

    bool isChecked = false;
    bool enabled = true;

    // Resets the play button to the replay button
    void changeCheckedImg(const char* pressedImg, const char* releasedImg);

    // When the play button is clicked
    void clickedPlay();
    // when the stop button is clicked
    void clickedStop();

// Emit the clicked() signal when the QLabel is clicked on
signals:
    void runTask();
    // Interrupts the running of the task
    void interrupt();
public slots:
    void enable();
    void disable();

protected:
    // Width and height
    int width;
    int height;
    double ratio;

    // Save both states of the image (clicked and not clicked)
    QPixmap pressed;
    QPixmap released;
    QPixmap pressed2;
    QPixmap released2;
    QPixmap disabled;

    void mousePressEvent(QMouseEvent *event) override {
        if (!enabled) { return; }
        if (!isChecked) { setPixmap(pressed); } else { setPixmap(pressed2); }
    }
    void mouseReleaseEvent(QMouseEvent *event) override {
        if (!enabled) { return; }
        if (!isChecked) { emit runTask(); } else { emit interrupt(); }
    }
};


#endif //SHOPIFY_BOT_CUSTOMWIDGETS_HPP
