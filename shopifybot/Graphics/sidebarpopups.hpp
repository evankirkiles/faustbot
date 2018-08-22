//
// Created by Evan Kirkiles on 8/1/18.
//

#ifndef SHOPIFY_BOT_SIDEBARPOPUPS_HPP
#define SHOPIFY_BOT_SIDEBARPOPUPS_HPP

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
#include <QCheckBox>
#ifndef QtConcurrent
#include <QtConcurrent>
#endif
#ifndef QFuture
#include <QFuture>
#endif
#ifndef QFutureWatcher
#include <QFutureWatcher>
#endif

#ifndef NewTaskDisplay
#include "popups.hpp"
#endif
#ifndef ClickableCheckableImage
#include "customwidgets.hpp"
#endif

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
    // Called when the credit card is submitted
    void submitted();
private slots:
    // Called whenever a new credit card is successfully added
    void submitEditOrNew();

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

    // Gets a unique name for the credit card
    std::string getSafeName(std::string currentTitle, std::string title = "Untitled");
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
    // Edits the information of a credit card
    void editCC();
    // Deletes a credit card profile
    void deleteCC();

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
    // Function that retrieves a qcombobox with all the titles of the credit cards for use in the profile
    void resetCreditCardQCB();
};

// Class for a QWidget item which is the standard on the proxy listwidget
class ProxyListItem : public QWidget {
    Q_OBJECT
public:
    // The index display
    QLabel* indexDisp;

    // Constructor that builds the custom ProxyListItem widget
    explicit ProxyListItem(QString index, QString ip, QString port, QString username, QString password,
                           QString settings = "", QWidget *parent = 0);
signals:
    // Emitted when the proxy check starts
    void proxyCheckStart();
    // Emitted when the proxy check ends
    void proxyCheckEnd();
public slots:
    // Refreshes the status image with proxy status
    void checkStatus();
private slots:
    // Uses the information in the proxy checker file to update the status
    void updateStatus();
    // Simply emits a signal from the class itself
    void proxyEndEmit();
private:
    // Several QLabels which simply display the passed in data
    QLabel* ipLabel;
    QLabel* portLabel;
    QLabel* usernameLabel;
    QLabel* passwordLabel;
    QLabel* statusIMG;

    // Pixmaps containing both states of the proxy connection status
    QPixmap proxyOn;
    QPixmap proxyOff;
    QPixmap proxyNeutral;

    // Process called in the separate thread
    void runCheck();
};

// The window which pops up when adding a proxy to the list
class AddProxyDisplay : public QWidget {
    Q_OBJECT
public:
    // Constructor that builds the add proxy window
    explicit AddProxyDisplay(int newIndex, ClickableCheckableImage* refresher,  QWidget* parent = 0);
    // Override the window closed
    void closeEvent(QCloseEvent* event) override;

signals:
    // Emitted when the add proxy display is closed
    void closed();
    // Emitted when a proxy has been added
    void submitted();
private slots:
    // Submits the information to a new proxy in the file
    void createNewProxy();
private:
    // The index at which to put the new proxy
    const int index;

    // Dark Title Bar widget
    DarkTitleBar* dtb;

    // Labels and their qlineedits for entering the proxy info
    QLabel* proxyIPLabel;
    QLineEdit* proxyIP;
    QLabel* proxyPortLabel;
    QLineEdit* proxyPort;
    QLabel* proxyUsernameLabel;
    QLineEdit* proxyUsername;
    QLabel* proxyPasswordLabel;
    QLineEdit* proxyPassword;

    // A clickable checkable image which can be used to deny updates
    ClickableCheckableImage* refreshButton;

    // Submit button which adds it to the list
    QPushButton* submit;
};

// Header for the Proxy Display window which will include a listview of the proxies and functions to add
// and delete them.
class ProxyDisplay : public QWidget {
    Q_OBJECT
public:
    // Constructor that builds the Proxy Window
    explicit ProxyDisplay(QWidget* parent = 0);
    // Override the window closed
    void closeEvent(QCloseEvent* event) override;

signals:
    // Emitted whenever the proxy display is closed
    void closed();
private slots:
    // Refreshes the proxy display
    void refresh(int selected);
    // Opens the add proxy display
    void openAddProxy();
    // Deletes the selected proxy
    void deleteProxy();
private:
    // Tells whether an add new proxy window is open
    bool addWindOpen = false;
    // Tells whether a thread is alive with a proxy check running
    int threadsOpen = 0;

    // Dark Title Bar widget
    DarkTitleBar* dtb;

    // Add new proxy window
    AddProxyDisplay* apd;

    // Icons for adding and deleting proxies
    QLabel* proxiesViewTitle;
    QLabel* proxyStatusCheck;
    ClickableImage* addProxyButton;
    ClickableImage* deleteProxyButton;
    ClickableCheckableImage* refreshProxies;
    ProxyListItem* columnProxies;

    // List view of all the proxies
    QListWidget* proxiesListView;
};

// Header for the URL Variant Parser window which gets all the products on a collection page or a product's page.
class ProductParserDisplay : public QWidget {
    Q_OBJECT
public:
    // Constructor that builds the parser window
    explicit ProductParserDisplay(QWidget *parent = 0);
    // Override the window closed
    void closeEvent(QCloseEvent* event) override;

signals:
    // Emitted whenever the proxy display is closed
    void closed();
private slots:
    // Parses the given fields and gets the products
    void parseProds();

private:
    // Dark title bar widget
    DarkTitleBar *dtb;

    // Left column widgets
    QLabel* parsedProductsLabel;
    QTextBrowser* parsedProducts;

    // Right column widgets
    QLabel* websiteLabel;
    QComboBox* websites;
    QLabel* typeLabel;
    QLabel* collectionLabel;
    QCheckBox* collection;
    QLabel* productLabel;
    QCheckBox* product;
    QLabel* extensionLabel;
    QLineEdit* extension;
    QLabel* limitLabel;
    QLineEdit* limit;
    QPushButton* goButton;
};

// Header for the settings window which allows the user to edit various features of the bot
class SettingsDisplay : public QWidget {
    Q_OBJECT
public:
    // Constructor that builds the settings display

};

#endif //SHOPIFY_BOT_SIDEBARPOPUPS_HPP
