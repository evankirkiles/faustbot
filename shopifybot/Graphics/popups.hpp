//
// Created by Evan Kirkiles on 7/23/18.
//

#ifndef SHOPIFY_BOT_POPUPS_HPP
#define SHOPIFY_BOT_POPUPS_HPP

#ifndef QWidget
#include <QWidget>
#endif
#ifndef QVBoxLayout
#include <QVBoxLayout>
#endif
#ifndef QHBoxLayout
#include <QHBoxLayout>
#endif
#ifndef QLabel
#include <QLabel>
#endif
#ifndef QPushButton
#include <QPushButton>
#endif
#ifndef QTextStream
#include <QTextStream>
#endif
#ifndef QFile
#include <QFile>
#endif
#ifndef QTextBrowser
#include <QTextBrowser>
#endif
#ifndef QFileInfo
#include <QFileInfo>
#endif

// Header containing all the declarations of the different popups available from the mainwindow. These include:
//  1. The log file display used for each individual task
//  2. The task information editor
//  3. The billing info editor
//  4. The proxy list editor
//  5. The information/about popup

// Class for a popup which displays a text file and refreshes it at a specified frequency.
class LogFileDisplay : public QWidget {
    Q_OBJECT
public:
    // Constructor that is given the file path for the file it is to open
    explicit LogFileDisplay(const std::string& title, const std::string& LFlocation, QWidget *parent = 0);

    // Override window closed event
    void closeEvent(QCloseEvent *event) override;

signals:
    // Signal emitted when the window is closed in the overridden closeevent
    void closed();
public slots:
    // Refresher slot that will be connected to a "FILE REFRESH" button, or can also be hooked up to a QTimer
    void refresh();
private:
    // File location is saved in a member string to allow for continuous refreshal
    const std::string title;

    // Widgets on the window
    QLabel* titledisplay;
    QPushButton* refreshButton;
    QFile* logFile;
    QTextStream* logStream;
    QTextBrowser* logDisplay;
protected:
    // Override the lose focus event so it closes whenever the window is left
    void focusOutEvent(QFocusEvent *event) override { close(); }
};

#endif //SHOPIFY_BOT_POPUPS_HPP
