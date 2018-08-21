//
// Created by Evan Kirkiles on 7/21/18.
//

#include "window.hpp"

// Constructor for the Bot's main window
BotWindow::BotWindow(QWidget *parent) : QWidget(parent) {
    // Set the size of the window
    setFixedSize(1100, 610);
    // Also set the stylesheet for the window
    QFile File("./shopifybot/Graphics/stylesheet.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    setStyleSheet(StyleSheet);
    // Finally, set some various window attributes
    setAttribute(Qt::WA_QuitOnClose);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowTitle(tr("SHOPIFY BOT"));
    setObjectName("main_window");

    // Create URLAndMethod metatype
    qRegisterMetaType<URLAndMethod>("URLAndMethod");

    // Build the Dark Title Bar
    dtb = new DarkTitleBar(this, true);

    // SIZE POLICIES
    // Size policies for top row
    QSizePolicy spRight(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spRight.setHorizontalStretch(3);

    // LAYOUTS
    // Create two vertical layouts within a horizontal main layout
    auto externLayout = new QVBoxLayout;
    externLayout->setContentsMargins(0, 0, 0, 0);
    auto inFrameLayout = new QVBoxLayout;
    inFrameLayout->setContentsMargins(0, 0, 0, 0);
    auto bg = new QFrame(this);
    bg->setObjectName("main_window");
    bg->setLayout(inFrameLayout);
    inFrameLayout->addWidget(dtb);
    auto mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(11, 3, 11, 11);
    auto topLayout = new QHBoxLayout;
    auto botLayout = new QHBoxLayout;
    // Vertical layouts for the top layout
    auto leftColumn = new QVBoxLayout;
    auto alltaskButtonsLayout = new QHBoxLayout;
    auto rightColumn = new QVBoxLayout;
    // Horizontal layout that goes above the listview
    auto tasktitleRow = new QHBoxLayout;
    // Vertical layout for the listview widget
    tasklistLayout = new QVBoxLayout;
    // Combine layouts
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(botLayout);
    topLayout->addLayout(leftColumn);
    topLayout->setStretchFactor(leftColumn, 1);
    inFrameLayout->addLayout(mainLayout);
    externLayout->addWidget(bg);
    setLayout(externLayout);

    // WIDGETS
    // Left column widgets
    QImage img(file_paths::LOGO_FILE);
    QPixmap logoimg;
    logoimg = QPixmap::fromImage(img).scaled(420, 400, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    logoimg.setDevicePixelRatio(2.0);
    logo = new QLabel();
    logo->setPixmap(logoimg);
    logo->setFixedSize(210, 200);
    title = new QLabel("SHOPIFY BOT", this);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("bot_title");
    title->setFixedHeight(30);
    startAllTasks = new QPushButton("Start All", this);
    startAllTasks->setObjectName("startallbutton");
    stopAllTasks = new QPushButton("Stop All", this);
    stopAllTasks->setObjectName("stopallbutton");
    alltaskButtonsLayout->addWidget(startAllTasks);
    alltaskButtonsLayout->addWidget(stopAllTasks);
    addtask = new QPushButton("NEW KEYWORD TASK", this);
    addtask->setObjectName("addtaskbutton");
    addtask->setFixedHeight(70);
    addVIDtask = new QPushButton("NEW VARIANT ID TASK", this);
    addVIDtask->setObjectName("addtaskbuttonv2");
    addVIDtask->setFixedHeight(50);
    billing = new QPushButton("Profiles", this);
    billing->setObjectName("sidebuttons");
    proxies = new QPushButton("Proxies", this);
    proxies->setObjectName("sidebuttons");
    logs = new QPushButton("Logs", this);
    logs->setObjectName("sidebuttons");
    copyrightLabel = new QLabel("© 2018 Evan Kirkiles - All Rights Reserved", this);
    copyrightLabel->setObjectName("copyrightlabel");
    // Add widgets to the left column
    leftColumn->addWidget(logo);
    leftColumn->addWidget(title);
    leftColumn->addLayout(alltaskButtonsLayout);
    leftColumn->addWidget(addtask);
    leftColumn->addWidget(addVIDtask);
    leftColumn->addWidget(billing);
    leftColumn->addWidget(proxies);
    leftColumn->addWidget(logs);
    leftColumn->addWidget(copyrightLabel);
    leftColumn->setAlignment(Qt::AlignTop);

    // Right column widget
    line1 = new QFrame();
    line1->setFrameShape(QFrame::HLine);
    line1->setFrameShadow(QFrame::Sunken);
    line1->setObjectName("horizontal_line");
    line2 = new QFrame();
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);
    line2->setObjectName("horizontal_line");
    tasktitle = new QLabel("TASKS", this);
    tasktitle->setAlignment(Qt::AlignCenter);
    tasktitle->setObjectName("task_title");
    tasklist = new QScrollArea(this);
    tasklist->setSizePolicy(spRight);
    tasklist->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tasklist->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tasklistwidget = new QWidget(this);

    tasklistLayout->setAlignment(Qt::AlignTop);
    tasklistwidget->setLayout(tasklistLayout);
    tasklistwidget->setObjectName("scrollarea");
    tasklist->setWidget(tasklistwidget);
    tasklist->setWidgetResizable(true);
    // Add widget to the right column
    tasktitleRow->addWidget(line1);
    tasktitleRow->addWidget(tasktitle);
    tasktitleRow->addWidget(line2);
    rightColumn->addLayout(tasktitleRow);
    rightColumn->addWidget(tasklist);
    topLayout->addLayout(rightColumn);
    topLayout->setStretchFactor(rightColumn, 4);

    // Open the add task window when the add task button is clicked
    connect(addtask, SIGNAL(clicked()), this, SLOT(openNewTask()));
    // Open the add VID task window when the add VID task buton is clicked
    connect(addVIDtask, SIGNAL(clicked()), this, SLOT(openNewVIDTask()));
    // Open the profiles window when the profiles button is clicked
    connect(billing, SIGNAL(clicked()), this, SLOT(openProfiles()));
    // Open the proxies window when the proxies button is clicked
    connect(proxies, SIGNAL(clicked()), this, SLOT(openProxies()));

    // PLACEHOLDER CONNECT
    connect(logs, SIGNAL(clicked()), this, SLOT(clearDirs()));

    // Build the timer updated on second intervals
    timeChecker = new QTimer(this);
    connect(timeChecker, &QTimer::timeout, [this] () { emit timeUpdated(QDateTime::currentDateTime()); } );
    timeChecker->setInterval(1000);
    timeChecker->start();
}

// Slot which takes information from the new task window and builds a task
void BotWindow::buildTask(QString title, URLAndMethod website, QString collection,
                          QString keywords, QString colorKeywords, QString size,
                          QDateTime start, QString profile, QString proxy, int copies) {

    // Gets an identifier by checking how many task widgets there are on the tasklist
    // Make sure to break the keywords QStrings down into its component keywords
    // Builds a task based on the number of copies sent
    for (int i=0; i<copies; i++) {
        addTask(title.toStdString(), website, std::to_string(numTasksCreated), collection.toStdString(),
                vectorFromString(keywords.toStdString()), vectorFromString(colorKeywords.toStdString()),
                size.toStdString(), start, profile.toStdString(), proxy.toStdString());
        // Increment the task identifier
        numTasksCreated++;
    }
}

// Slot which takes information from the new VID task window and builds necessary VID tasks
void BotWindow::buildVIDTask(QString title, URLAndMethod website, QString variantID, QString variantName,
                             QString variantSize, QDateTime start, QString profile, QString proxy,
                             QString p_imageURL, int copies) {

    // Gets an identifier by checking how many task widgets there are on the tasklist
    // Builds a task based on the number of copies sent
    for (int i = 0; i < copies; i++) {
        addVIDTaskFunc(title.toStdString(), website, std::to_string(numTasksCreated), variantID.toStdString(),
                       variantName.toStdString(), variantSize.toStdString(), start, profile.toStdString(),
                       proxy.toStdString(), p_imageURL);
        // Increment the task identifier
        numTasksCreated++;
    }
}

// Creates a task and adds it to the tasklist
void BotWindow::addTask(const std::string &title, const URLAndMethod &website, const std::string &identifier,
                        const std::string &collection, const std::vector<std::string> &keywords,
                        const std::vector<std::string> &colorKeywords, const std::string &size,
                        const QDateTime& startAt, const std::string& profile, const std::string& proxy,
                        unsigned int resultsToCheck, unsigned int frequency) {

    // Create a new task
    auto newtask = new TaskWidget(title, website, identifier, collection, keywords, colorKeywords, size,
                                        startAt, profile, proxy, &logWindowOpen, &editWindowOpen,
                                        resultsToCheck, frequency, tasklistwidget);

    // Connect the signals of the window to this new task's start and stop
    connect(startAllTasks, SIGNAL(clicked()), newtask, SLOT(run()));
    connect(stopAllTasks, SIGNAL(clicked()), newtask, SLOT(stopWidget()));

    // Connect the 1 second interval timer to the taskwidget's time check function
    connect(this, SIGNAL(timeUpdated(QDateTime)), newtask, SLOT(checkTime(QDateTime)));

    // Adds the task to the qvboxlayout
    tasklistLayout->addWidget(newtask);
    // Show the new task
    newtask->show();
}

// Creates a tasl and adds it to the tasklist
void BotWindow::addVIDTaskFunc(const std::string &title, const URLAndMethod &website, const std::string &identifier,
                               const std::string &variantID, const std::string &variantName,
                               const std::string &variantSize, const QDateTime &startAt, const std::string &profile,
                               const std::string &proxy, QString p_imageURL, unsigned int frequency) {

    // Create a new task
    auto newtask = new VIDTaskWidget(title, website, identifier, variantID, profile, proxy, startAt, variantName,
            variantSize, p_imageURL, &logWindowOpen, &editWindowOpen, frequency, tasklistwidget);

    // Connect the signals of the window to this new task's start and stop
    connect(startAllTasks, SIGNAL(clicked()), newtask, SLOT(run()));
    connect(stopAllTasks, SIGNAL(clicked()), newtask, SLOT(stopWidget()));

    // Connect the 1 second interval timer to the taskwidget's time check function
    connect(this, SIGNAL(timeUpdated(QDateTime)), newtask, SLOT(checkTime(QDateTime)));

    // Add the task to the qvboxlayout
    tasklistLayout->addWidget(newtask);
    // Show the new task
    newtask->show();

}

// Opens the add task window
void BotWindow::openNewTask() {
    if (addTaskOpen) {
        // Shopw the currently open task window
        atd->raise();
        atd->setFocus();
        return;
    }

    // If there is a VID new task open, just close it
    if (addVIDTaskOpen) { avidtd->close(); }

    // Build the window if it does not exist, otherwise just show it
    atd = new AddTaskDisplay();
    atd->show();
    addTaskOpen = true;

    // Make necessary connections
    connect(atd, &AddTaskDisplay::closed, [this] () { addTaskOpen = false; });
    // Connect the buildtask symbol to the addtask slot of the mainwindow
    connect(atd, SIGNAL(sendTask(QString, URLAndMethod, QString, QString, QString, QString, QDateTime, QString, QString, int)),
            this, SLOT(buildTask(QString, URLAndMethod, QString, QString, QString, QString, QDateTime, QString, QString, int)));
}

// Opens the add VID task window
void BotWindow::openNewVIDTask() {
    if (addVIDTaskOpen) {
        // Show the currently open task window
        avidtd->raise();
        avidtd->setFocus();
        return;
    }

    // If there is a normal new task open, just close it
    if (addTaskOpen) { atd->close(); }

    // Build the window if it does not exist, otherwise just show it
    avidtd = new AddVIDTaskDisplay();
    avidtd->show();
    addVIDTaskOpen = true;

    // Make necessary connections
    connect(avidtd, &AddVIDTaskDisplay::closed, [this] () { addVIDTaskOpen = false; });
    // Connect the buildtask symbol to the addtask slot of the mainwindow
    connect(avidtd, SIGNAL(sendTask(QString, URLAndMethod, QString, QString, QString, QDateTime, QString, QString, QString, int)),
            this, SLOT(buildVIDTask(QString, URLAndMethod, QString, QString, QString, QDateTime, QString, QString, QString, int)));
}

// Opens the profiles window
void BotWindow::openProfiles() {
    if (profilesOpen) {
        // Show the currently open profiles window
        profilesDisp->raise();
        profilesDisp->setFocus();
        return;
    }
    // Build the window if it does not exist
    profilesDisp = new ProfilesDisplay();
    profilesDisp->show();
    profilesOpen = true;

    // Make necessary connections
    connect(profilesDisp, &ProfilesDisplay::closed, [this] () { profilesOpen = false; });
}

// Opens the proxies window
void BotWindow::openProxies() {
    if (proxiesOpen) {
        // Show the currently open proxies window
        proxiesDisp->raise();
        proxiesDisp->setFocus();
        return;
    }
    // Build the window if it does not exist
    proxiesDisp = new ProxyDisplay();
    proxiesDisp->show();
    proxiesOpen = true;

    // Make necessary connections
    connect(proxiesDisp, &ProxyDisplay::closed, [this] () { proxiesOpen = false; });
}

// Clears all the temporary file directories
void BotWindow::clearDirs() {

    // Remove the contents directory and recreate it and its subdirectory cookiejar
    boost::filesystem::remove_all(file_paths::CONTENTS_DIR);
    boost::filesystem::create_directory(file_paths::CONTENTS_DIR);
    boost::filesystem::create_directory(std::string(file_paths::CONTENTS_DIR).append("/CookieJar"));

    // Removes the logs directory and recreates it
    boost::filesystem::remove_all(file_paths::LOGS_DIR);
    boost::filesystem::create_directory(file_paths::LOGS_DIR);
}