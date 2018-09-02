//
// Created by Evan Kirkiles on 9/1/18.
//

#include "authentication.hpp"

// Builds the authentication popup
AuthenticationPopup::AuthenticationPopup(QWidget *parent) :
        db(QSqlDatabase::addDatabase("QMYSQL")),
        stack(new QStackedWidget(this)),
        firstpage(new QWidget(this)),
        secondpage(new QWidget(this)),
        authToken(new QLineEdit(this)),
        authStatus(new QLabel("Authenticating...", this)),
        returnButton(new ClickableCheckableImage(26, 26, 2, file_paths::REFRESH2_IMG, file_paths::REFRESH_IMG,
                                                 file_paths::REFRESH2_IMG, file_paths::REFRESH_IMG,
                                                 file_paths::DISABLEDREFRESH_IMG, this)),
        submit(new QPushButton("SUBMIT", this)),
        QWidget(parent) {

    // Set the size of the window
    setFixedSize(300, 75);
    // Set the stylesheet for the window
    QFile File(file_paths::STYLESHEET);
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    QFile File2(QApplication::applicationDirPath().append(file_paths::COLORSTYLESHEET).toStdString().c_str());
    File2.open(QFile::ReadOnly);
    QString CStyleSheet = QLatin1String(File2.readAll());
    setStyleSheet(StyleSheet + CStyleSheet);
    // Finally, set some various window attributes
    setAttribute(Qt::WA_QuitOnClose, false);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowTitle(tr("Authentication Token"));
    setObjectName("main_window");

    dtb = new DarkTitleBar(this);
    dtb->mainLayout->removeWidget(dtb->closeWindow);
    delete dtb->closeWindow;

    // LAYOUTS
    // Create two vertical layouts within a horizontal main layout
    auto externLayout = new QVBoxLayout;
    externLayout->setContentsMargins(0, 0, 0, 0);
    auto internLayout = new QVBoxLayout;
    internLayout->setContentsMargins(0, 0, 0, 0);
    internLayout->setSpacing(0);
    auto bg = new QFrame(this);
    bg->setObjectName("main_window");
    auto mainLayout = new QVBoxLayout;
    internLayout->addWidget(dtb);
    mainLayout->setContentsMargins(11, 10, 11, 11);
    mainLayout->addWidget(stack);
    internLayout->addLayout(mainLayout);
    internLayout->addStretch();
    bg->setLayout(internLayout);

    // First page of widgets
    auto authTokenLayout = new QHBoxLayout;
    authTokenLayout->setContentsMargins(0, 0, 0, 0);
    authToken->setObjectName("addtask_editbox");
    authToken->setFixedHeight(20);
    authTokenLayout->addWidget(authToken);
    authTokenLayout->addSpacing(15);
    authTokenLayout->addWidget(submit);
    submit->setObjectName("addtaskbutton");
    submit->setFixedSize(60, 30);
    firstpage->setLayout(authTokenLayout);
    stack->addWidget(firstpage);

    // Second page of widgets
    auto authstatusLayout = new QHBoxLayout;
    authstatusLayout->addWidget(authStatus);
    authstatusLayout->addWidget(returnButton);
    authstatusLayout->setAlignment(Qt::AlignCenter);
    authstatusLayout->setContentsMargins(0, 0, 0, 0);
    authStatus->setFixedHeight(20);
    secondpage->setLayout(authstatusLayout);
    stack->addWidget(secondpage);

    // Connect to mySQL server
    db.setHostName("den1.mysql3.gear.host");
    db.setDatabaseName("faustbot");
    db.setUserName("faustbot");
    db.setPassword("Ha5u~6UQtL0?");

    // Finalize layouts
    externLayout->addWidget(bg);
    setLayout(externLayout);

    // Connect submit to checking the ID authenticity
    connect(submit, &QPushButton::clicked, [this] () {
        if (authToken->text().isEmpty()) { authToken->setFocus(); return; }
        checkAuthAvailability();
        changeStackIndex(1);
    });
    // Connect the return button to returning to the first page
    connect(returnButton, &ClickableCheckableImage::runTask, [this] () {
        changeStackIndex(0);
    });

    // Perform the user authentication
    // Check if there is an auth token available
    std::ifstream authTokenFile(QApplication::applicationDirPath().append(file_paths::AUTHENTICATION).toStdString().c_str());
    // If file does not exist or could not be opened, then reask for auth token
    if (authTokenFile.fail()) {
        stack->setCurrentIndex(0);
    // Otherwise check validity of authentication token
    } else {
        std::string token;
        getline(authTokenFile, token);
        // If token exists, then use it to try to authenticate with server
        if (!token.empty()) {
            stack->setCurrentIndex(1);
            returnButton->disable();
            authenticate(token);
        } else {
            stack->setCurrentIndex(0);
        }
    }
    // Close the file
    authTokenFile.close();
}

// Changes the stack index with a nice fade effect
void AuthenticationPopup::changeStackIndex(int to) {

    // Fade the stack out
    // Fade out the container
    if (to == 1) {
        firstpage->setDisabled(true);
        returnButton->enable();
        auto *eff = new QGraphicsOpacityEffect(this);
        firstpage->setGraphicsEffect(eff);
        QPropertyAnimation *a = new QPropertyAnimation(eff, "opacity");
        a->setDuration(250);
        a->setStartValue(1);
        a->setEndValue(0.1);
        a->setEasingCurve(QEasingCurve::Linear);
        a->start(QPropertyAnimation::DeleteWhenStopped);

        // Wait for the QPropertyAnimation to finish
        connect(a, &QPropertyAnimation::finished, [this] () {
            stack->setCurrentIndex(1);
            fadePage(secondpage, true);
        });
    } else if (to == 0) {
        secondpage->setDisabled(true);
        returnButton->disable();
        auto *eff = new QGraphicsOpacityEffect(this);
        secondpage->setGraphicsEffect(eff);
        QPropertyAnimation *a = new QPropertyAnimation(eff, "opacity");
        a->setDuration(250);
        a->setStartValue(1);
        a->setEndValue(0.1);
        a->setEasingCurve(QEasingCurve::Linear);
        a->start(QPropertyAnimation::DeleteWhenStopped);

        // Wait for the QPropertyAnimation to finish
        connect(a, &QPropertyAnimation::finished, [this] () {
            stack->setCurrentIndex(0);
            fadePage(firstpage, true);
        });
    }
}

// Fades in or out a widget
void AuthenticationPopup::fadePage(QWidget *which, bool inOrOut) {
    if (inOrOut) {
        which->setDisabled(false);
        auto *eff = new QGraphicsOpacityEffect(this);
        which->setGraphicsEffect(eff);
        QPropertyAnimation *a = new QPropertyAnimation(eff, "opacity");
        a->setDuration(250);
        a->setStartValue(0);
        a->setEndValue(1);
        a->setEasingCurve(QEasingCurve::Linear);
        a->start(QPropertyAnimation::DeleteWhenStopped);
    } else {
        which->setDisabled(true);
        auto *eff = new QGraphicsOpacityEffect(this);
        which->setGraphicsEffect(eff);
        QPropertyAnimation *a = new QPropertyAnimation(eff, "opacity");
        a->setDuration(250);
        a->setStartValue(1);
        a->setEndValue(0);
        a->setEasingCurve(QEasingCurve::Linear);
        a->start(QPropertyAnimation::DeleteWhenStopped);
    }
}

// Checks the database at the given authentication token to see if a machine is already registered under that ID
void AuthenticationPopup::checkAuthAvailability() {

    // TODO: Write a function to get the special, unique computer hash which is put into a global string
    std::string hashcode = "asdsadgysahuidjhuaskijaukhsugsjfdhdhiqfw8diguask";

    // Get the text from the QLineEdit
    std::string authTokenString = authToken->text().toStdString();
    authTokenString.erase (std::remove (authTokenString.begin(), authTokenString.end(), '\''), authTokenString.end());

    // Now set up connection to MySQL database
    bool ok = db.open();

    // If connection is made, then look in the database for the given authToken
    if (ok) {
        authStatus->setText("Connected, checking token...");

        // Perform SQL query
        QSqlQuery query;
        query.exec(std::string("SELECT machine_hash FROM auth WHERE auth_token = '").append(authTokenString).append("'").c_str());
        if (query.next()) {
            // Check if the hash code is already set for the given ID
            if (query.value(0).toString().toStdString().empty()) {
                // If it is not, then set it
                query.clear();
                query.exec(std::string("UPDATE auth SET machine_hash = '").append(hashcode).append("' WHERE auth_token='").append(authTokenString).append("'").c_str());

                // Write the authentication token to the authentication file location
                std::ofstream fileout(QApplication::applicationDirPath().append(file_paths::AUTHENTICATION).toStdString().c_str(), std::ios::out | std::ios::trunc);
                fileout << authTokenString;
                fileout.close();

                // Now quit the database connection
                db.close();

                // Finally, close the authentication because it succesfully added a row
                authStatus->setText("Authenticated!");
                emit closed();
                close();
                return;
            } else {
                authStatus->setText("Authentication token already in use.");
            }
        } else {
            authStatus->setText("Invalid authentication token!");
        }
    }

    // Now quit the database connection
    db.close();
}

// Tries to authenticate the computer
void AuthenticationPopup::authenticate(const std::string& authTokenString) {

    // Local instance of machine hash string (will be empty)
    std::string hashcode = "asdsadgysahuidjhuaskijaukhsugsjfdhdhiqfw8digu";

    // Ensure connection to auth server
    bool ok = db.open();

    // If connection is made, then look in the database for the given authToken
    if (ok) {
        authStatus->setText("Connected, checking token...");

        // Perform SQL query
        QSqlQuery query;
        query.exec(std::string("SELECT machine_hash FROM auth WHERE auth_token = '").append(authTokenString).append("'").c_str());
        if (query.next()) {
            // If there is a result returned from the select, then use its machine_hash
            std::string machine_hash = query.value(0).toString().toStdString();

            std::cout << machine_hash << " : "  << hashcode << std::endl;

            // Now compare the current hash code against the database's stored hash code for the given ID
            if (machine_hash == hashcode) {
                std::cout << "TRUF" << std::endl;
                // If they are equal, then the app is authenticated
                // Close the connection to the SQL database
                db.close();
                emit closed();
                close();
                return;
            } else {
                // Otherwise the app is not authenticated
                authStatus->setText("Unauthorized machine for the vault token!");
                returnButton->enable();
            }
        }
    } else {
        // In case the thing doesn't work, do something?
        authStatus->setText("Couldn't connect to authentication servers.");
    }

    // Close the connection to the SQL database
    db.close();
}