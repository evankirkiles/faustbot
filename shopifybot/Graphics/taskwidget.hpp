//
// Created by Evan Kirkiles on 7/22/18.
//

#ifndef SHOPIFY_BOT_TASKWIDGET_HPP
#define SHOPIFY_BOT_TASKWIDGET_HPP

#ifndef QWidget
#include <QWidget>
#endif
#ifndef Task
#include "../Infrastructure/tasks.hpp"
#endif
#ifndef QLabel
#include <QLabel>
#endif
#ifndef QVBoxLayout
#include <QVBoxLayout>
#endif
#ifndef QHBoxLayout
#include <QHBoxLayout>
#endif
#ifndef QTextEdit
#include <QTextEdit>
#endif
#ifndef QPushButton
#include <QPushButton>
#endif
#ifndef QPainter
#include <QPainter>
#endif
#ifndef QColor
#include <QColor>
#endif
#ifndef QThread
#include <QThread>
#endif

#include "popups.hpp"

// Custom clickable label to allow for images to be pressed and checked
class ClickableCheckableImage : public QLabel {
Q_OBJECT
public:
    // Default constructor and destructors
    ClickableCheckableImage(int p_width, int p_height, const char* pressedImg, const char* releasedImg,
                            const char* pressedImg2, const char* releasedImg2, const char* disabledImg,
                            QWidget *parent = 0) :
            width(p_width), height(p_height),
            pressed(QPixmap::fromImage(QImage(pressedImg).scaled(p_width, p_height, Qt::KeepAspectRatio, Qt::SmoothTransformation))),
            released(QPixmap::fromImage(QImage(releasedImg).scaled(p_width, p_height, Qt::KeepAspectRatio, Qt::SmoothTransformation))),
            pressed2(QPixmap::fromImage(QImage(pressedImg2).scaled(p_width, p_height, Qt::KeepAspectRatio, Qt::SmoothTransformation))),
            released2(QPixmap::fromImage(QImage(releasedImg2).scaled(p_width, p_height, Qt::KeepAspectRatio, Qt::SmoothTransformation))),
            disabled(QPixmap::fromImage(QImage(disabledImg).scaled(p_width, p_height, Qt::KeepAspectRatio, Qt::SmoothTransformation))),
            QLabel(parent) {
        pressed.setDevicePixelRatio(4.0);
        pressed2.setDevicePixelRatio(4.0);
        released.setDevicePixelRatio(4.0);
        released2.setDevicePixelRatio(4.0);
        disabled.setDevicePixelRatio(4.0);

        // Set the default image to be the released, not checked option
        setPixmap(released);
        setMaximumWidth(width/4);
    }

    bool isChecked = false;
    bool enabled = true;

    // Resets the play button to the replay button
    void changeCheckedImg(const char* pressedImg, const char* releasedImg) {
        pressed = QPixmap::fromImage(QImage(pressedImg).scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        released = QPixmap::fromImage(QImage(releasedImg).scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        pressed.setDevicePixelRatio(4.0);
        released.setDevicePixelRatio(4.0);
    }

// Emit the clicked() signal when the QLabel is clicked on
signals:
    void runTask();
    // Interrupts the running of the task
    void interrupt();
public slots:
    void enable() {
        setPixmap(released);
        enabled = true;
    };
    void disable() {
        setPixmap(disabled);
        enabled = false;
    }
protected:
    // Width and height
    int width;
    int height;

    // Save both states of the image (clicked and not clicked)
    QPixmap pressed;
    QPixmap released;
    QPixmap pressed2;
    QPixmap released2;
    QPixmap disabled;

    void mousePressEvent(QMouseEvent *event) override {
        if (!enabled) {
            return;
        }
        if (!isChecked) {
            // Change to the pressed image
            setPixmap(pressed);
        } else {
            setPixmap(pressed2);
        }
    }
    void mouseReleaseEvent(QMouseEvent *event) override {
        if (!enabled) {
            return;
        }
        if (!isChecked) {
            setPixmap(released2);
            isChecked = true;
            emit runTask();
        } else {
            // Make the label go back to its initial image
            setPixmap(released);
            isChecked = false;
            emit interrupt();
        }
    }
};

// Custom vertical label class
class VerticalLabel : public QLabel {
    Q_OBJECT
public:
    explicit VerticalLabel(QWidget *parent=0) : QLabel(parent) {}
    explicit VerticalLabel(const QString &text, QWidget *parent=0) : QLabel(text, parent) {}
protected:
    void paintEvent(QPaintEvent*) override {
        QPainter painter(this);
        painter.setPen(QColor("#e8cb61"));
        painter.setBrush(Qt::Dense1Pattern);

        painter.translate( sizeHint().width(), sizeHint().height() );
        painter.rotate(270);

        painter.drawText(QPoint(0, 0), text());
    }
    QSize sizeHint() const override {
        QSize s = QLabel::minimumSizeHint();
        return QSize(s.height(), s.width());
    }
    QSize minimumSizeHint() const override {
        QSize s = QLabel::sizeHint();
        return QSize(s.height(), s.width());
    }
};

// Contains the class that builds a Task widget to be added to the scroll area
class TaskWidget : public QFrame {
Q_OBJECT
public:
    // Constructor
    explicit TaskWidget(const std::string& title, const URLAndMethod& website, const std::string& identifier,
                        const std::string& collection, const std::vector<std::string>& keywords,
                        const std::vector<std::string>& colorKeywords, const std::string& size, unsigned int quantity,
                        bool* p_logWindowOpen, unsigned int resultsToCheck=constants::BASE_NUMRESULTS,
                        unsigned int frequency=constants::BASE_FREQ, QWidget *parent = 0);

private slots:
    // Runs the task, performed when user clicks the play button
    void run();
    // Updates the status with the given message for the given color
    void setStatus(QString text, QString hexColor);
    // Deletes the slot, but only if there no thread currently running
    void exit();
    // Builds a logfile window with the task's title and file specifiers
    void showLogs();
    // Called when the logfile window is closed by user
    void logsClosed();

private:
    // Actual task instance
    Task task;

    // Thread for copied task
    QThread* taskthread;

    // Logfile window
    LogFileDisplay* lfd;

    // Labels on the task
    QLabel* title;
    QLabel* website;
    QLabel* collection;
    QFrame* separator1;
    QTextEdit* keywords;
    QTextEdit* colorKeywords;
    QLabel* size;
    QLabel* quantity;
    QFrame* separator2;

    // Play and edit buttons
    ClickableCheckableImage* play;
    ClickableImage* edit;

    // Status label and logs button
    QTextEdit* status;
    bool* logWindowOpen;
    ClickableImage* logsButton;

    // Delete button
    ClickableImage* deleteButton;
};

#endif //SHOPIFY_BOT_TASKWIDGET_HPP
