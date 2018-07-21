//
// Created by Evan Kirkiles on 7/21/18.
//

#ifndef SHOPIFY_BOT_WINDOW_HPP
#define SHOPIFY_BOT_WINDOW_HPP

// Qt includes
#ifndef QWidget
#include <QWidget>
#endif
#ifndef QFile
#include <QFile>
#endif
#ifndef QString
#include <QString>
#endif

// Qt Window through which user is able to interact with the bot.
class BotWindow : public QWidget {

    Q_OBJECT
public:
    // Constructor that builds the window
    explicit BotWindow(QWidget *parent = 0);
};

#endif //SHOPIFY_BOT_WINDOW_HPP
