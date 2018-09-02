//
// Created by Evan Kirkiles on 9/1/18.
//

#include "authentication.hpp"

// Builds the authentication popup
AuthenticationPopup::AuthenticationPopup(QWidget *parent) :
        stack(new QStackedWidget(this)),
        firstpage(new QWidget(this)),
        secondpage(new QWidget(this)),
        authToken(new QLineEdit(this)),
        authStatus(new QLabel("STATUS", this)),
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
    authStatus->setFixedHeight(20);
    secondpage->setLayout(authstatusLayout);
    stack->addWidget(secondpage);

    // Sets the current stack index
    stack->setCurrentIndex(0);

    // Finalize layouts
    externLayout->addWidget(bg);
    setLayout(externLayout);

    // Connect submit to closing the thing
    connect(submit, &QPushButton::clicked, [this] () { emit closed(); close(); });
}