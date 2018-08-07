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
#ifndef algorithm
#include <algorithm>
#endif
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// Window which pops up when adding a new Credit Card
class AddCreditCardDisplay : public QWidget {
    Q_OBJECT
public:
    // Constructor that builds the add credit card window
    explicit AddCreditCardDisplay(QString profiletitle = QString(""), QWidget *parent = 0);
    // Override the window closed
    void closeEvent(QCloseEvent* event) override;

signals:
    // Called whenever the add credit card display is closed
    void closed();
    // Called whenever a new credit card is successfully added
    void submitted();

private:
    // The title of the credit card; if exists then edit task rather than add task
    QString ccprofiletitle;

    // Dark Title bar widget
    DarkTitleBar* dtb;

    // Widgets to edit the credit card
    QLabel* titleLabel;
    QLineEdit* title;
    QLabel* ccnumLabel;
    QLineEdit* ccnum;
    QLabel* ccnameLabel;
    QLineEdit* ccname;
    QLabel* ccdateLabel;
    QDateEdit *ccdate;
    QLabel* ccccvLabel;
    QLineEdit* ccccv;

    // The push button to submit the credit card
    QPushButton* submit;

    // Function to fill in the credit card information being edited
    void fillCCInfo();
};

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
    // Changes the text fields to the selected profile's values
    void select(QString which);
    // Submits the text fields and overwrites their respective positions in the profiles.txt file
    void submit();
    // Creates a new profile called Untitled
    void createNew();
    // Duplicates the selected profile
    void duplicateProfile();
    // Deletes the selected profile
    void deleteProfile();
    // Refreshes the profile list
    void refresh(int selected = 0);
    // Refreshes the credit card list
    void refreshCC(int selected = 0);
    // Builds the add credit card display
    void addCC();

private:
    // Specifies whether the moreInfoDisplay is open or not
//    bool moreInfoDisplayOpen = false;
//    MoreInfoDisplay* mid;

    // Dark Title bar widget
    DarkTitleBar* dtb;

    // The Add Credit Card display
    AddCreditCardDisplay* accd;
    bool accdOpen = false;

    // Icons for adding, duplicating, and deleting profiles
    QLabel* profileViewTitle;
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

    // The next row of small buttons and the credit card label
    QLabel* creditcardViewTitle;
    ClickableImage* addCreditCardButton;
    ClickableImage* deleteCreditCardButton;
    ClickableImage* editCreditCardButton;

    // List view of all the credit cards currently opened in the left column
    QListWidget* creditcardsListView;

    // Function that returns a 'safe' version of a name (is unique)
    std::string getSafeName(bool addingnew = false, int currentIndex = 0, std::string title = "Untitled");
};

#endif //SHOPIFY_BOT_SIDEBARPOPUPS_HPP
