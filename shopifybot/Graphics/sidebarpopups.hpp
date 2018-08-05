//
// Created by Evan Kirkiles on 8/1/18.
//

#ifndef SHOPIFY_BOT_SIDEBARPOPUPS_HPP
#define SHOPIFY_BOT_SIDEBARPOPUPS_HPP

#ifndef NewTaskDisplay
#include "popups.hpp"
#endif

#ifndef QListWidget
#include <QListWidget>
#endif
#ifndef ifstream
#include <fstream>
#endif
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

// Header for the side bar popups, including billing information, proxy information, etc.
class ProfilesDisplay : public QWidget {
    Q_OBJECT
public:
    // Constructor that builds the profiles window
    explicit ProfilesDisplay(QWidget *parent = 0);
    // Override the window closed
    void closeEvent(QCloseEvent* event) override;

signals:
    // Called whenever the profiles display is closed
    void closed();
private slots:
    // Changes the text fields to the selected profile's values and readies the
    void select(QString which);
    // Refreshes the profile list
    void refresh();

private:
    // Specifies whether the moreInfoDisplay is open or not
//    bool moreInfoDisplayOpen = false;
//    MoreInfoDisplay* mid;

    // Dark Title bar widget
    DarkTitleBar* dtb;

    // Icons for adding, duplicating, and deleting profiles
    ClickableImage* addProfileButton;
    ClickableImage* duplicateProfileButton;
    ClickableImage* deleteProfileButton;

    // List view of all the profiles currently opened in the left column
    QListWidget* profilesListView;

    // All the lineedits and the labels for editing the profiles
    QLabel* titleLabel;
    QLineEdit* editTitle;
    QLabel* emailLabel;
    QLineEdit* email;
    QLabel* firstnameLabel;
    QLineEdit* firstname;
    QLabel* lastnameLabel;
    QLineEdit* lastname;
    QLabel* address1Label;
    QLineEdit* address1;
    QLabel* address2Label;
    QLineEdit* address2;
    QLabel* cityLabel;
    QLineEdit* city;
    QLabel* countryLabel;
    QLineEdit* country;
    QLabel* provinceLabel;
    QLineEdit* province;
    QLabel* zipcodeLabel;
    QLineEdit* zipcode;
    QLabel* phoneLabel;
    QLineEdit* phone;
    QLabel* ccardLabel;
    QComboBox* ccard;
    QPushButton* update;
};

#endif //SHOPIFY_BOT_SIDEBARPOPUPS_HPP
