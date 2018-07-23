//
// Created by Evan Kirkiles on 7/21/18.
//

#ifndef SHOPIFY_BOT_WINDOW_HPP
#define SHOPIFY_BOT_WINDOW_HPP

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

// Include the task widget item
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

    // Adds a task to the list
    void addTask(const std::string& title, const URLAndMethod& website, const std::string& collection,
                 const std::string& identifier, const std::vector<std::string>& keywords,
                 const std::vector<std::string>& colorKeywords,
                 const std::string& size, unsigned int quantity,
                 unsigned int resultsToCheck=constants::BASE_NUMRESULTS, unsigned int frequency=constants::BASE_FREQ);

private:

    // Visualization widgets
    QLabel *logo;
    QLabel *title;
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

    // Add a task when button is clicked
    void testtask();
};

#endif //SHOPIFY_BOT_WINDOW_HPP
