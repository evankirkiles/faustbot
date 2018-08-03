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

// Include the task class
#include "../Infrastructure/tasks.hpp"

// Include the widgets
#include "taskwidget.hpp"
// Include the sidebar popups
#include "sidebarpopups.hpp"

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
    void addTask(const std::string& title, const URLAndMethod& website, const std::string& collection,
                 const std::string& identifier, const std::vector<std::string>& keywords,
                 const std::vector<std::string>& colorKeywords, const std::string& size, const QDateTime& startAt,
                 const std::string& profile, const std::string& proxy,
                 unsigned int resultsToCheck=constants::BASE_NUMRESULTS, unsigned int frequency=constants::BASE_FREQ);

private:

    // Dark title bar widget
    DarkTitleBar* dtb;

    // Bool telling whether windows are open
    bool addTaskOpen = false;
    bool profilesOpen = false;
    // Number of tasks created, incremented every time a task is added
    int numTasksCreated = 0;

    // Visualization widgets
    QLabel *logo;
    QLabel *title;

    // Start all tasks and stop all tasks button
    QPushButton* startAllTasks;
    QPushButton* stopAllTasks;

    // Add new task button
    QPushButton *addtask;
    AddTaskDisplay* atd;

    // Other sidebar buttons
    QPushButton *billing;
    QPushButton *proxies;
    QPushButton *logs;
    QPushButton *about;
    QLabel *copyrightLabel;

    // Sidebar windows
    ProfilesDisplay* profilesDisp;

    // Tasks listview
    QFrame *line1;
    QLabel* tasktitle;
    QFrame *line2;
    QScrollArea *tasklist;
    QWidget* tasklistwidget;
    QVBoxLayout *tasklistLayout;

private slots:
    // Opens the add task window
    void openNewTask();
    // Tells the main window that the add task window was closed
    void addTaskClosed();
    // Opens the profiles window
    void openProfiles();
    // Tells the main window that the profiles window was closed
    void profilesClosed();
    // Connects to the submit slot in the main window, and adds a task with the given information
    void buildTask(QString title, URLAndMethod website, QString collection,
                   QString keywords, QString colorKeywords, QString size,
                   QDateTime start, QString profile, QString proxy, int copies);
};



#endif //SHOPIFY_BOT_WINDOW_HPP
