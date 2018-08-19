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
#ifndef QComboBox
#include <QComboBox>
#endif
#ifndef QLineEdit
#include <QLineEdit>
#endif
#ifndef QDateTimeEdit
#include <QDateTimeEdit>
#endif
#ifndef QIntValidator
#include <QIntValidator>
#endif
#ifndef QGraphicsOpacityEffect
#include <QGraphicsOpacityEffect>
#endif
#ifndef QPropertyAnimation
#include <QPropertyAnimation>
#endif
#ifndef ifstream
#include <fstream>
#endif

#include "titlebar.hpp"

// Header containing all the declarations of the different popups available from the mainwindow. These include:
//  1. The log file display used for each individual task
//  2. The task information editor
//  3. The billing info editor
//  4. The proxy list editor
//  5. The information/about popup

// Window that pops up when you click the moreinfo button
class MoreInfoDisplay : public QWidget {
Q_OBJECT
public:
    // Constructor that builds the window, params denote the image to be used and the size of the window
    explicit MoreInfoDisplay(unsigned int width, unsigned int height, const char* img_filepath, QWidget *parent = 0);
    // Override the window closed event
    void closeEvent(QCloseEvent *event) override;

signals:
    // Called whenever the moreinfo display is closed
    void closed();

public slots:
    // Transparent fade in and fade out of the More Info window
    void fadeIn();
    void fadeOut();

private:
    // Instance-related variables
    QLabel* imageLabel;

    // Set the background as a simple gray qframe
    QFrame* background;

protected:
    // Override the lose focus event so it closes whenever the window is left
    void focusOutEvent(QFocusEvent *event) override { close(); }
};

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

    // Dark title bar
    DarkTitleBar* dtb;

    // Widgets on the window
    QFile* logFile;
    QTextStream* logStream;
    QTextBrowser* logDisplay;
};

// Window that pops up when you try to add a task
class AddTaskDisplay : public QWidget {
    Q_OBJECT
public:
    // Constructor that builds the window
    explicit AddTaskDisplay(QWidget *parent = 0);

    // Override window closed event
    void closeEvent(QCloseEvent *event) override;
signals:
    // Called whenever the addtask window is closed
    void closed();
    // Sends the information of the new task to the main window to add it
    void sendTask(QString title, URLAndMethod website, QString collection,
                  QString keywords, QString colorKeywords, QString size,
                  QDateTime start, QString profile, QString proxy, int numCopies);
private slots:
    // Tries to send the information in the form to the main window to add it to the task list
    // If not all required fields are filled, then does not send the form yet.
    void attemptToSend();
    // Opens the moreInfoDisplay which gives a basic tutorial on how to create a new task
    void buildMoreInfoDisplay();
    // Called whenever the moreinfodisplay window is closed
    void MIDClosed();

private:

    // Specifies whether the moreInfoDisplay is open or not
    bool moreInfoDisplayOpen = false;
    MoreInfoDisplay* mid;

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
    QLabel* copiesLabel;
    QLineEdit* copies;

    // Profile and proxy row
    QLabel* profileLabel;
    QComboBox* profile;
    QLabel* proxyLabel;
    QComboBox* proxy;

    // Title row is last with the finished button
    QLabel* titleLabel;
    QLineEdit* title;
    QPushButton* submit;

    // Builds the qcombobox used for profiles
    void buildProfilesBox();
    // Builds the qcombobox used for proxies
    void buildProxiesBox();
};

// Window that pops up when you try to add a task
class AddVIDTaskDisplay : public QWidget {
    Q_OBJECT
public:
    // Constructor that builds the window
    explicit AddVIDTaskDisplay(QWidget *parent = 0);

    // Override window closed event
    void closeEvent(QCloseEvent *event) override;
signals:
    // Called whenever the addtask window is closed
    void closed();
    // Sends the information of the new task to the main window to add it
    void sendTask(QString title, URLAndMethod website, QString variantID,
                  QString variantName, QString variantSize, QDateTime start, QString profile,
                  QString proxy, int numCopies);
private slots:
    // Tries to send the information in the form to the main window to add it to the task list
    // If not all required fields are filled, then does not send the form yet.
    void attemptToSend();
    // Opens the moreInfoDisplay which gives a basic tutorial on how to create a new task
    void buildMoreInfoDisplay();
    // Called whenever the moreinfodisplay window is closed
    void MIDClosed();

private:

    // Specifies whether the moreInfoDisplay is open or not
    bool moreInfoDisplayOpen = false;
    MoreInfoDisplay* mid;

    // Dark title bar widget
    DarkTitleBar* dtb;

    // The widgets through which the new task will  be added to the client
    // Website and variant ID row
    QLabel* websitesLabel;
    QComboBox* websites;
    QLabel* variantIDLabel;
    QLineEdit* variantID;
    QPushButton* checkForNameButton;

    // Row for title
    QLabel* variantTitleLabel;
    QLineEdit* variantTitle;
    QLabel* variantSizeLabel;
    QLineEdit* variantSize;

    // Start at, proxy, and profile row
    QLabel* startAtLabel;
    QDateTimeEdit* startAt;
    QLabel* profileLabel;
    QComboBox* profile;
    QLabel* proxyLabel;
    QComboBox* proxy;

    // Finished button row is last
    // Title row is last with the finished button
    QLabel* titleLabel;
    QLineEdit* title;
    QLabel* copiesLabel;
    QLineEdit* copies;
    QPushButton* submit;

    // Builds the profile combobox
    void buildProfiles();
    // Builds the proxy combobox
    void buildProxies();
};

#endif //SHOPIFY_BOT_POPUPS_HPP