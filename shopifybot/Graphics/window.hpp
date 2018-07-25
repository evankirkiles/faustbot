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

    // Adds a task to the list
    void addTask(const std::string& title, const URLAndMethod& website, const std::string& collection,
                 const std::string& identifier, const std::vector<std::string>& keywords,
                 const std::vector<std::string>& colorKeywords,
                 const std::string& size, unsigned int quantity,
                 unsigned int resultsToCheck=constants::BASE_NUMRESULTS, unsigned int frequency=constants::BASE_FREQ);

private:

    // Dark title bar widget
    DarkTitleBar* dtb;

    // Bool telling whether add task window is open
    bool addTaskOpen = false;

    // Visualization widgets
    QLabel *logo;
    QLabel *title;

    // Add new task button
    QPushButton *addtask;
    AddTaskDisplay* atd;

    // Other sidebar buttons
    QPushButton *billing;
    QPushButton *proxies;
    QPushButton *logs;
    QPushButton *about;

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
    // Add a task when button is clicked
    void testtask();
};

// Function which interprets the string form of a vector back into the vector
// Ex. "A,B A,C D W" will produce a vector of size 3 with elements "A", "B A", "C D W"
std::vector<std::string> vectorFromString(const std::string& interpret);

#endif //SHOPIFY_BOT_WINDOW_HPP
