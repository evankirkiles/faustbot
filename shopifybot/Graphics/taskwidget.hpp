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

    // When the play button is clicked
    void clickedPlay() {
        setPixmap(released2);
        isChecked = true;
    }

    // when the stop button is clicked
    void clickedStop() {
        // Make the label go back to its initial image
        setPixmap(released);
        isChecked = false;
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
            emit runTask();
        } else {
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


// Window that pops up when you try to edit a task
// Put it in this class and not the popups because needs to have a reference to its TaskWidget parent
class EditTaskDisplay : public QWidget {
Q_OBJECT
public:
    // Constructor that builds the window
    explicit EditTaskDisplay(const QString& title, const QString& website, const QString& collection,
                             const QString& keywords, const QString& colorKeywords, const QString& size,
                             const QDateTime& start, const QString& profile, const QString& proxy, QWidget* parent = 0);
    // Override window closed event to notify main window when current edit task is open
    void closeEvent(QCloseEvent *event) override;

signals:
    // Called whenever the edit task window closes
    void closed();
    // Sends the edited task information back to the task to restructure
    void sendTaskEdit(QString title, URLAndMethod website, QString collection,
                      QString keywords, QString colorKeywords, QString size,
                      QDateTime start, QString profile, QString proxy);

private slots:
    // Tries to send the information in the form to the task widget to edit the task.
    // If not all required fields are filled, then does not send the form.
    void attemptToSend();

private:
    // Dark title bar widget
    DarkTitleBar* dtb;

    // The widgets through which the new task will be added to the client
    // Website and collection row
    QLabel* websitesLabel;
    QComboBox* websites;
    QLabel* collectionLabel;
    QLineEdit* collection;

    // Keyword row
    QLabel* keywordsLabel;
    QLineEdit* keywords;

    // Color Keyword row
    QLabel* colorKeywordsLabel;
    QLineEdit* colorKeywords;

    // Size and time row
    QLabel* sizeLabel;
    QLineEdit* size;
    QLabel* etaLabel;
    QDateTimeEdit* eta;

    // Profile and proxy row
    QLabel* profileLabel;
    QComboBox* profile;
    QLabel* proxyLabel;
    QComboBox* proxy;

    // Finished button row is last
    // Title row is last with the finished button
    QLabel* titleLabel;
    QLineEdit* title;
    QPushButton* cancel;
    QPushButton* submit;

    // Builds the profile combobox
    void buildProfiles();
    // Builds the proxy combobox
    void buildProxies();
};

// Contains the class that builds a Task widget to be added to the scroll area
class TaskWidget : public QFrame {
Q_OBJECT
public:
    // Constructor
    explicit TaskWidget(const std::string& title, const URLAndMethod& website, const std::string& identifier,
                        const std::string& collection, const std::vector<std::string>& keywords,
                        const std::vector<std::string>& colorKeywords, const std::string& size, const QDateTime& startAt,
                        const std::string& profile, const std::string& proxy, bool* p_logWindowOpen, bool* p_editWindowOpen,
                        unsigned int resultsToCheck=constants::BASE_NUMRESULTS,
                        unsigned int frequency=constants::BASE_FREQ, QWidget *parent = 0);

    // Title label, public so edit task can access it
    QLabel* title;

signals:
    // Emitted to the running task to tell it to stop
    void stopTask();

private slots:
    // Runs the task, performed when user clicks the play button
    void run();
    // Stops the task, performed when user clicks the stop button
    void stopWidget();
    // Updates the status with the given message for the given color
    void setStatus(QString text, QString hexColor);
    // Deletes the slot, but only if there no thread currently running
    void exit();
    // Builds a logfile window with the task's title and file specifiers
    void showLogs();
    // Called when the logfile window is closed by user
    void logsClosed();
    // Builds the edit window with all the task's information
    void showEdit();
    // Receives the task edit from the window
    void acceptTaskEdit(QString title, URLAndMethod website, QString collection,
                        QString keywords, QString colorKeywords, QString size,
                        QDateTime start, QString profile, QString proxy);
    // Called when the edit window is closed/cancelled by user
    void editClosed();

private:
    // Actual task instance
    Task* task;

    // Thread for copied task
    QThread* taskthread;

    // Logfile window
    LogFileDisplay* lfd;

    // Edit window
    EditTaskDisplay* etd;

    // Labels on the task
    QLabel* website;
    QLabel* collection;
    QFrame* separator1;
    QTextEdit* keywords;
    QTextEdit* colorKeywords;
    QLabel* size;
    QDateTimeEdit* startAt;
    QFrame* separator2;

    // Play and edit buttons
    ClickableCheckableImage* play;
    ClickableImage* edit;
    bool* editWindowOpen;

    // Status label and logs button
    QTextEdit* status;
    bool* logWindowOpen;
    ClickableImage* logsButton;

    // Delete button
    ClickableImage* deleteButton;
};

#endif //SHOPIFY_BOT_TASKWIDGET_HPP
