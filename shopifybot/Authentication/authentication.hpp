//
// Created by Evan Kirkiles on 9/1/18.
//

#ifndef FAUST_AUTHENTICATION_HPP
#define FAUST_AUTHENTICATION_HPP

// Includes for the darken graphics effect
#include <QGraphicsEffect>
#include <QPainter>
#include <QPixmap>
#include <QPoint>
#include <QTransform>

// Includes for the authentication popup
#include <QWidget>
#include <QObject>
#include <QLineEdit>
#include <QPushButton>
#include <QFile>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QPropertyAnimation>

// QtSQL includes
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

// Guarded includes
#ifndef ifstream
#include <fstream>
#endif
#ifndef algorithm
#include <algorithm>
#endif

// Include the constants
#include "constants.hpp"
// And include the clickablecheckableimage
#include "Graphics/customwidgets.hpp"
// Also the dark title bar
#include "Graphics/titlebar.hpp"
// Also the address generator
#include "Authentication/addressgenerator.hpp"

// The QGraphicsEffect to put over the window
class DarkenEffect : public QGraphicsEffect {
public:
    // Override the draw function
    void draw(QPainter* painter) override {
        QPixmap pixmap;
        QPoint offset;
        // No point in drawing in device coordinates (pixmap will be scaled anyways)
        if (sourceIsPixmap()) {
            pixmap = sourcePixmap(Qt::LogicalCoordinates, &offset);
        } else {
            pixmap = sourcePixmap(Qt::DeviceCoordinates, &offset);
            painter->setWorldTransform(QTransform());
        }
        // Base color is black
        painter->setBrush(QColor(0, 0, 0, 255));
        painter->drawRoundRect(pixmap.rect(), 2, 3);
        painter->setOpacity(0.25);
        painter->drawPixmap(offset, pixmap);
    }
};

// The Authentication pop up which checks the product's authentication id
class AuthenticationPopup : public QWidget {
    Q_OBJECT
public:
    // Constructor to build the authentication popup
    explicit AuthenticationPopup(QWidget *parent = 0);

    bool authenticated = false;

signals:
    // Tells whether the user is authorized
    void closed();
private slots:
    // Checks if there exists a machine has for the given authentication ID in the database
    void checkAuthAvailability();
    // Checks if the computer matches the one in the database for the given authentication ID
    void authenticate(const std::string& token);
private:
    // Stacked Widget for differnet pages of authentication popup
    QStackedWidget* stack;
    QWidget* firstpage;
    QWidget* secondpage;

    // The Dark Title Bar
    DarkTitleBar* dtb;

    // Widgets on the authentication popup window
    // First page
    QLineEdit* authToken;
    QPushButton* submit;

    // Second page
    QLabel* authStatus;
    ClickableCheckableImage* returnButton;

    // Connection to the mySQL server
    QSqlDatabase db;

    // Changes the index of the stack
    void changeStackIndex(int to);
    // Fades a widget
    void fadePage(QWidget* which, bool inOrOut);
};


#endif //FAUST_AUTHENTICATION_HPP
