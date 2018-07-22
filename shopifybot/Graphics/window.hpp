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
#include <QListView>

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
    QListView *tasklist;
};

#endif //SHOPIFY_BOT_WINDOW_HPP
