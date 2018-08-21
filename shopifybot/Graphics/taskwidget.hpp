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
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QBitmap>

#ifndef ClickableCheckableImage
#include "customwidgets.hpp"
#endif
#include "popups.hpp"

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
    QPushButton* submit;

    // Builds the profile combobox
    void buildProfiles();
    // Builds the proxy combobox
    void buildProxies();
};

// Window that pops up when you try to edit a variant ID task
class VIDTaskEditDisplay : public QWidget {
    Q_OBJECT
public:
    // Constructor that builds the window with the given information
    explicit VIDTaskEditDisplay(const QString& title, const QString& website, const QString& variantID, const QString& variantName,
                                const QString& variantSize, const QDateTime& start, const QString& profile,
                                const QString& proxy, QWidget* parent = 0);

    // Override winow closed event to notify main window when current edit task is open
    void closeEvent(QCloseEvent* event) override;
signals:
    // Called whenever the edit task window closes
    void closed();
    // Sends the edited task information back to the task to restructure
    void sendTaskEdit(QString title, URLAndMethod website, QString variantId, QString variantName, QString variantSize,
                      QDateTime startAt, QString profile, QString proxy, QString imageURL, unsigned int frequency = constants::BASE_FREQ);

private slots:
    // Tries to send the information in the form to the task widget to edit the task.
    // If not all required fields are filled, then does not send the form.
    void attemptToSend();
    // Retrieves the variant ID's name, size, and image from the given website
    void fillFromVariant();

private:
    QString imageURL;

    // Dark title bar widget
    DarkTitleBar* dtb;

    // The widgets through which the new task will  be added to the client
    // Website and variant ID row
    QLabel* websitesLabel;
    QComboBox* websites;
    QLabel* variantIDLabel;
    QLineEdit* variantID;
    QPushButton* checkForNameButton;

    // Row for title and size
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

public slots:
    // Run every time the main window's QTimer updates
    void checkTime(QDateTime time);
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
                        QDateTime start, QString profile, QString proxy, unsigned int frequency = constants::BASE_FREQ);
    // Called when the edit window is closed/cancelled by user
    void editClosed();

private:
    bool deleted = false;

    // Frequency of the task
    unsigned int taskfreq;

    // Actual task instance
    Task* task;

    // Thread for copied task
    QThread* taskthread;

    // Logfile window
    LogFileDisplay* lfd;

    // Edit window
    EditTaskDisplay* etd;

    // Labels on the task
    QLabel* identifier;
    QFrame* separator0;
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

// Task widget for a variant-ID specific task
class VIDTaskWidget : public QFrame {
    Q_OBJECT
public:
    // Constructor
    explicit VIDTaskWidget(const std::string& title, const URLAndMethod& website, const std::string& variantID,
                           const std::string& identifier, const std::string& profile, const std::string& proxy,
                           const QDateTime& startAt, const std::string &prodTitle, const std::string& prodSize,
                           const QString& imageURL, bool* p_logWindowOpen, bool* p_editWindowOpen,
                           unsigned int frequency, QWidget* parent = 0);

    // Title label, public so edit task can access it
    QLabel* title;

signals:
    // Emitted to the running task to tell it to stop
    void stopTask();
public slots:
    // Run every time the main window's QTimer updates
    void checkTime(QDateTime time);
private slots:
    // Runs the task, performed when user clicks the play button
    void run();
    // Stops the task, performed when user clicks the stop button
    void stopWidget();
    // Updates the status with the given message for the given color
    void setStatus(QString text, QString hexColor);
    // Deletes the slot, but only if there is no thread currently running
    void exit();
    // Builds a logfile window with the task's title and file specifiers
    void showLogs();
    // Called when the logfile window is closed by the user
    void logsClosed();
    // Builds the edit window with all the task's information
    void showEdit();
    // Receives the task edit from the window
    void acceptTaskEdit(QString title, URLAndMethod website, QString variantID, QString variantName, QString variantSize,
                        QDateTime start, QString profile, QString proxy, QString imageURL, unsigned int frequency = constants::BASE_FREQ);
    // Called when the edit window is closed
    void editClosed();

    // Retrieves a network reply containig the image data
    void downloadFinished(QNetworkReply* reply);

private:
    bool deleted = false;

    // Store information about the thumbnail image
    QString imageURL;
    QPixmap thumbnail;
    QPixmap defaultThumbnail;

    // Frequency of the while loop
    unsigned int taskfreq;

    // Variant task ID instance
    VariantIDTask* task;

    // Thread for copied task
    QThread* taskthread;

    // Logfile window
    LogFileDisplay* lfd;

    // Edit window
    VIDTaskEditDisplay* etd;

    // Labels on the task
    QLabel* identifier;
    QFrame* separator0;
    QLabel* website;
    QLabel* variantId;
    QFrame* separator1;
    QLabel* thumbnailImg;
    QTextEdit* variantName;
    QLabel* variantSize;
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

    // Loads in an image from the imageURL and stores in the thumbnail, which is then put on the QLabel
    void loadImage();
};

#endif //SHOPIFY_BOT_TASKWIDGET_HPP
