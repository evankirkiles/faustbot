//
// Created by Evan Kirkiles on 7/21/18.
//

#ifndef SHOPIFY_BOT_WINDOW_HPP
#define SHOPIFY_BOT_WINDOW_HPP

// std includes
#ifndef stringstream
#include <sstream>
#endif

// Qt includes
#include <QWidget>
#include <QFile>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGraphicsView>
#include <QLabel>
#include <QPixmap>
#include <QBitmap>
#include <QTimer>

// Include the task class
#include "../Infrastructure/tasks.hpp"

// Include the widgets
#include "taskwidget.hpp"
// Include the sidebar popups
#include "sidebarpopups.hpp"

#include "Authentication/authentication.hpp"

// Constants include
#ifndef file_paths
#include "constants.hpp"
#endif

// Qt Window through which user is able to interact with the bot.
class BotWindow : public QWidget {

    Q_OBJECT
public:
    // Constructor that builds the window
    explicit BotWindow(QWidget *parent = 0);

    // Tells if a logwindow is open
    bool logWindowOpen = false;
    // Tells if a task edit window is open
    bool editWindowOpen = false;

    // Adds a task to the list
    void addTask(const std::string& title, const URLAndMethod& website, const std::string& identifier,
                 const std::string& collection, const std::vector<std::string>& keywords,
                 const std::vector<std::string>& colorKeywords, const std::string& size, const QDateTime& startAt,
                 const std::string& profile, const std::string& proxy,
                 unsigned int resultsToCheck=constants::BASE_NUMRESULTS, unsigned int frequency=constants::BASE_FREQ);

    // Adds a VID task to the list
    void addVIDTaskFunc(const std::string& title, const URLAndMethod& website, const std::string& identifier,
                        const std::string& variantID, const std::string& variantName, const std::string& variantSize,
                        const QDateTime& startAt, const std::string& profile, const std::string& proxy, QString imageURL,
                        unsigned int frequency = constants::BASE_FREQ);

    // Checks the authentication
    void checkAuthentication();
signals:
    // Emitted every time the qtimer timesout
    void timeUpdated(QDateTime time);
    // Emitted to delete all the tasks
    void deleteAllTasks();
public slots:
    // Tells the window that the application is not authorized
    void receiveAuthentication();
    // Builds the Authentication window
    void buildAuthWindow();
    // Loads in the tasks from the tasks file
    void loadInSavedTasks();
private:

    // Dark title bar widget
    DarkTitleBar* dtb;

    // Authorization popup
    AuthenticationPopup* atp;

    // Bool telling whether windows are open
    bool addTaskOpen = false;
    bool addVIDTaskOpen = false;
    bool parserOpen = false;
    bool profilesOpen = false;
    bool proxiesOpen = false;
    bool settingsOpen = false;
    // Number of tasks created, incremented every time a task is added
    int numTasksCreated = 0;

    // Container widget
    QWidget* container;

    // Visualization widgets
    QLabel *logo;
    QLabel *title;

    // Start all tasks and stop all tasks button
    QPushButton* startAllTasks;
    QPushButton* stopAllTasks;

    // Add new task button
    QPushButton *addtask;
    AddTaskDisplay* atd;
    QPushButton *addVIDtask;
    AddVIDTaskDisplay* avidtd;

    // Other sidebar buttons
    QPushButton* variantParser;
    QPushButton *billing;
    QPushButton *proxies;
    QPushButton *clear;
    QPushButton *saveTasks;
    QPushButton *loadTasks;
    QPushButton *settings;
    SettingsDisplay *settingsDisp;
    QLabel *copyrightLabel;

    // Sidebar windows
    ProductParserDisplay* parserDisp;
    ProfilesDisplay* profilesDisp;
    ProxyDisplay* proxiesDisp;

    // Tasks listview
    QFrame *line1;
    QLabel* tasktitle;
    QFrame *line2;
    QScrollArea *tasklist;
    QWidget* tasklistwidget;
    QVBoxLayout *tasklistLayout;

    // Timer which keeps track of task start times
    QTimer* timeChecker;

private slots:
    // Opens the add task window
    void openNewTask();
    // Opens an add VID task window
    void openNewVIDTask();
    // Opens the parser window
    void openParser();
    // Opens the profiles window
    void openProfiles();
    // Opens the proxies window
    void openProxies();
    // Opens the settings window
    void openSettings();
    // Connects to the submit slot in the new task window, and adds a task with the given information
    void buildTask(QString title, URLAndMethod website, QString collection,
                   QString keywords, QString colorKeywords, QString size,
                   QDateTime start, QString profile, QString proxy, int copies);
    // Connects to the submit slot in the new VID task window, and adds a VID task with the given information
    void buildVIDTask(QString title, URLAndMethod website, QString variantID, QString variantName, QString variantSize,
                      QDateTime start, QString profile, QString proxy, QString p_imageURL, int copies);
};



#endif //SHOPIFY_BOT_WINDOW_HPP
