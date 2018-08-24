//
// Created by Evan Kirkiles on 7/23/18.
//

#include <shopifybot/WebAccess/product_scraper.hpp>
#include "popups.hpp"

// LOG FILE DISPLAY CLASS
// Constructor that builds the window
LogFileDisplay::LogFileDisplay(const std::string& p_title, const std::string& LFlocation, QWidget *parent) :
        title(p_title), QWidget(parent) {

    // Fixed size of the window
    setFixedSize(500, 300);
    setWindowTitle(std::string(p_title + " Logs").c_str());
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setObjectName("logs_window");
    setAttribute(Qt::WA_QuitOnClose, false);

    // Set the stylesheet for the window
    QFile File(file_paths::STYLESHEET);
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    QFile File2(QApplication::applicationDirPath().append(file_paths::COLORSTYLESHEET).toStdString().c_str());
    File2.open(QFile::ReadOnly);
    QString CStyleSheet = QLatin1String(File2.readAll());
    setStyleSheet(StyleSheet + CStyleSheet);

    // Initialize dark title bar
    dtb = new DarkTitleBar(this, false, true);
    auto externLayout = new QVBoxLayout;
    externLayout->setContentsMargins(0, 0, 0, 0);
    auto inFrameLayout = new QVBoxLayout;
    inFrameLayout->setContentsMargins(0, 0, 0, 0);
    auto bg = new QFrame(this);
    bg->setObjectName("main_window");
    bg->setLayout(inFrameLayout);
    inFrameLayout->addWidget(dtb);

    // Create the window layouts
    auto mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(11, 3, 11, 11);
    inFrameLayout->addLayout(mainLayout);
    externLayout->addWidget(bg);

    // Initialize the logFile and the textstream
    logFile = new QFile(LFlocation.c_str());
    logFile->open(QIODevice::ReadOnly);
    if (logFile->exists()) {
        logStream = new QTextStream(logFile);
        logDisplay = new QTextBrowser();
        logDisplay->setText(logStream->readAll());
        QScrollBar* vsb = logDisplay->verticalScrollBar();
        vsb->setValue(vsb->maximum());
    } else {
        logDisplay = new QTextBrowser();
        logDisplay->setText(QString("Log file not yet generated."));
    }

    logDisplay->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    logDisplay->setObjectName("logs_text");

    // Add these things to the main layout
    mainLayout->addWidget(logDisplay);

    // Set the window's layout
    setLayout(externLayout);

    // Connect the refresh button
    connect(dtb->refreshButton, SIGNAL(clicked()), this, SLOT(refresh()));
}

// Refresh slot which reloads the filestream into the textbrowser
void LogFileDisplay::refresh() {
    logFile->close();
    logFile->open(QIODevice::ReadOnly);
    if (logFile->exists()) {
        logStream = new QTextStream(logFile);
        logDisplay->setText(logStream->readAll());
        QScrollBar* vsb = logDisplay->verticalScrollBar();
        vsb->setValue(vsb->maximum());
    } else {
        logDisplay->setText(QString("Log file not yet generated."));
    }
}

// Custom close event function that just emits a signal signifying it has closed
void LogFileDisplay::closeEvent(QCloseEvent *event) {
    // Emit the closed signal and then proceed to cleanup
    emit closed();
    QWidget::closeEvent(event);
}

// ADD TASK DISPLAY CLASS
// Constructor that builds the window for adding a task
AddTaskDisplay::AddTaskDisplay(QWidget *parent) : QWidget(parent) {

    // Set window properties
    setFixedSize(500, 270);
    setObjectName("newtaskwindow");
    setWindowTitle("New Keyword Task");
    setWindowFlags(Qt::FramelessWindowHint);
    setFocusPolicy(Qt::ClickFocus);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_TranslucentBackground);

    // Create the title bar
    dtb = new DarkTitleBar(this, true);

    // Set the stylesheet for the window
    QFile File(file_paths::STYLESHEET);
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    QFile File2(QApplication::applicationDirPath().append(file_paths::COLORSTYLESHEET).toStdString().c_str());
    File2.open(QFile::ReadOnly);
    QString CStyleSheet = QLatin1String(File2.readAll());
    setStyleSheet(StyleSheet + CStyleSheet);

    // Create layouts
    auto externLayout = new QVBoxLayout();
    externLayout->setContentsMargins(0, 0, 0, 0);
    auto bg = new QFrame(this);
    auto bgLayout = new QVBoxLayout();
    bgLayout->setContentsMargins(0, 0, 0, 0);
    bg->setObjectName("main_window");
    bg->setLayout(bgLayout);
    bgLayout->addWidget(dtb);
    bgLayout->addStretch();
    externLayout->addWidget(bg);
    auto mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(11, 3, 11, 11);
    // Individual horizontal row layouts
    auto websiteCollectionLayout = new QHBoxLayout();
    auto keywordLayout = new QHBoxLayout();
    auto colorKeywordLayout = new QHBoxLayout();
    auto sizeEtaLayout = new QHBoxLayout();
    auto frequencyLayout = new QHBoxLayout();
    auto titleLayout = new QHBoxLayout();

    // Create the widgets
    // WEBSITE & COLLECTION ROW
    websitesLabel = new QLabel("Website: ", this);
    websitesLabel->setObjectName("addtask_mediocre_text");
    websites = new QComboBox(this);
    websites->addItems(supported_sites::ssStringList);
    collectionLabel = new QLabel("Collection: ", this);
    collectionLabel->setObjectName("addtask_mediocre_text");
    collection = new QLineEdit(this);
    collection->setObjectName("addtask_editbox");
    collection->setPlaceholderText("/collections/footwear");
    // Add row to the layout
    websiteCollectionLayout->addWidget(websitesLabel);
    websiteCollectionLayout->addWidget(websites);
    websiteCollectionLayout->addWidget(collectionLabel);
    websiteCollectionLayout->addWidget(collection);
    mainLayout->addLayout(websiteCollectionLayout);

    // KEYWORD ROW
    keywordsLabel = new QLabel("Title Keywords: ", this);
    keywordsLabel->setObjectName("addtask_mediocre_text");
    keywords = new QLineEdit(this);
    keywords->setObjectName("addtask_editbox");
    // Add row to the layout
    keywordLayout->addWidget(keywordsLabel);
    keywordLayout->addWidget(keywords);
    mainLayout->addLayout(keywordLayout);

    // COLOR KEYWORD ROW
    colorKeywordsLabel = new QLabel("Color Keywords: ", this);
    colorKeywordsLabel->setObjectName("addtask_mediocre_text");
    colorKeywords = new QLineEdit(this);
    colorKeywords->setObjectName("addtask_editbox");
    // Add row to the layout
    colorKeywordLayout->addWidget(colorKeywordsLabel);
    colorKeywordLayout->addWidget(colorKeywords);
    mainLayout->addLayout(colorKeywordLayout);

    // SIZE & ETA & COPIES ROW
    sizeLabel = new QLabel("Size: ", this);
    sizeLabel->setObjectName("addtask_mediocre_text");
    sizeLabel->setMaximumWidth(35);
    size = new QLineEdit(this);
    size->setObjectName("addtask_editbox");
    size->setMaximumWidth(35);
    etaLabel = new QLabel("Start at: ", this);
    etaLabel->setObjectName("addtask_mediocre_text");
    etaLabel->setMaximumWidth(60);
    eta = new QDateTimeEdit(this);
    eta->setObjectName("addtask_datetime");
    eta->setDisplayFormat("[MMMM d, yyyy] hh:mm:ss");
    // Format the time to minute 00
    QDateTime thecurrent = QDateTime::currentDateTime();
    QTime thetime = thecurrent.time();
    if (thetime.second() != 0) { thetime.setHMS(thetime.hour(), thetime.minute(), 0); }
    thecurrent.setTime(thetime);
    eta->setDateTime(thecurrent);
    copiesLabel = new QLabel("Copies: ", this);
    copiesLabel->setObjectName("addtask_mediocre_text");
    copiesLabel->setMaximumWidth(45);
    copies = new QLineEdit(this);
    copies->setObjectName("addtask_editbox");
    copies->setValidator(new QIntValidator(0, 20, this));
    copies->setText("1");
    copies->setMaximumWidth(35);
    // Add row to the layout
    sizeEtaLayout->addStretch();
    sizeEtaLayout->addWidget(sizeLabel);
    sizeEtaLayout->addWidget(size);
    sizeEtaLayout->addWidget(etaLabel);
    sizeEtaLayout->addWidget(eta);
    sizeEtaLayout->addWidget(copiesLabel);
    sizeEtaLayout->addWidget(copies);
    sizeEtaLayout->addStretch();
    mainLayout->addLayout(sizeEtaLayout);

    // PROFILES & PROXY ROW
    profileLabel = new QLabel("Profile: ", this);
    profileLabel->setObjectName("addtask_mediocre_text");
    profileLabel->setMaximumWidth(50);
    profile = new QComboBox(this);
    profile->setFixedWidth(100);
    buildProfilesBox();
    proxyLabel = new QLabel("Proxy: ", this);
    proxyLabel->setObjectName("addtask_mediocre_text");
    proxyLabel->setMaximumWidth(45);
    proxy = new QComboBox(this);
    proxy->setFixedWidth(75);
    buildProxiesBox();
    // Add row to the layout
    frequencyLayout->addStretch();
    frequencyLayout->addWidget(profileLabel);
    frequencyLayout->addWidget(profile);
    frequencyLayout->addWidget(proxyLabel);
    frequencyLayout->addWidget(proxy);
    frequencyLayout->addStretch();
    mainLayout->addLayout(frequencyLayout);

    // TITLE ROW
    titleLabel = new QLabel("Title: ", this);
    titleLabel->setObjectName("task_important_text");
    title = new QLineEdit(this);
    title->setObjectName("task_title_lineedit");
    submit = new QPushButton("SUBMIT", this);
    submit->setObjectName("addtaskbutton");
    submit->setFixedSize(100, 35);
    // Add row to the layout
    titleLayout->addWidget(titleLabel);
    titleLayout->addWidget(title);
    titleLayout->addSpacing(10);
    titleLayout->addWidget(submit);
    mainLayout->addLayout(titleLayout);

    // Set the layout
    bgLayout->addLayout(mainLayout);
    setLayout(externLayout);

    // Connect the submit button to the attemptToSend slot
    connect(submit, SIGNAL(clicked()), this, SLOT(attemptToSend()));
    // Connect the moreinfo button the moreinfodisplay slot
    connect(dtb, SIGNAL(showMIW()), this, SLOT(buildMoreInfoDisplay()));
}



// Tries to send the data in the input fields to the main window to build a new task
void AddTaskDisplay::attemptToSend() {

    // Non-required fields: collection, color keywords
    // Check each required input field to make sure a valid task can be built
    if (keywords->text().isEmpty()) { keywords->setFocus(); return; }
    if (title->text().isEmpty()) { title->setFocus(); return; }

    // Send the signal with all the data to the main window if all required fields have text in them
    emit sendTask(title->text(), supported_sites::WEBSITES.at(websites->currentText().toStdString()), collection->text(),
                  keywords->text(), colorKeywords->text(), size->text(), eta->dateTime(), profile->currentText(),
                  proxy->currentText(), copies->text().toInt());

    // Now close the window because the new task has been made
    close();
}

// Opens the moreinfo display to give a basic tutorial on how to create a new task
void AddTaskDisplay::buildMoreInfoDisplay() {

    // Make sure that a tutorial display is not open already
    if (moreInfoDisplayOpen) { if (mid) { mid->raise(); } return; }

    // Otherwise create a tutorial display and show it
    mid = new MoreInfoDisplay(400, 400, file_paths::MOREINFO_IMG);
    mid->fadeIn();
    mid->setFocus();

    // Connect the close situations
    connect(submit, SIGNAL(clicked()), mid, SLOT(fadeOut()));
    connect(this, SIGNAL(closed()), mid, SLOT(fadeOut()));
    connect(dtb, SIGNAL(hideMIW()), mid, SLOT(fadeOut()));
    // Also notify the add new task window when the window is closed
    connect(mid, SIGNAL(closed()), this, SLOT(MIDClosed()));

    // More info bool set to true
    moreInfoDisplayOpen = true;
}

// Function that simply changes the boolean when the more info display is closed
void AddTaskDisplay::MIDClosed() {
    moreInfoDisplayOpen = false;
}

// Custom close event function that just emits a signal signifying it has closed
void AddTaskDisplay::closeEvent(QCloseEvent *event) {
    // Emit the closed signal and then proceed to cleanup
    emit closed();
    QWidget::closeEvent(event);
}

// Scans the profiles.txt file for the titles of each profile
void AddTaskDisplay::buildProfilesBox() {
    // First add "Random" as an option
    profile->addItem("Random");

    // Open the profiles.txt
    std::ifstream filein(QApplication::applicationDirPath().append(file_paths::PROFILES_TXT).toStdString().c_str());
    std::string tempStr;

    // Cycle through each line and get the profiles' titles
    while (getline(filein, tempStr)) {
        profile->addItem(tempStr.substr(0, tempStr.find(" :-:")).c_str());
    }

    // Close the file in
    filein.close();
}

// Scans the proxies.txt file for the IPs of each proxies
void AddTaskDisplay::buildProxiesBox() {
    // First add "Random" as an option
    proxy->addItem("Random");

    // Open the proxies.txt
    std::ifstream filein(QApplication::applicationDirPath().append(file_paths::PROXIES_TXT).toStdString().c_str());
    std::string tempStr;

    // Cycle through each line and get the proxies' ips
    while (getline(filein, tempStr)) {
        proxy->addItem(tempStr.substr(0, tempStr.find(" :-: ")).c_str());
    }

    // Close the filein
    filein.close();
}

// ADD VARIANT ID TASK DISPLAY CLASS
// Constructor that builds the window for adding a task
AddVIDTaskDisplay::AddVIDTaskDisplay(QWidget *parent) : QWidget(parent) {

    // Set window properties
    setFixedSize(500, 230);
    setObjectName("newtaskwindow");
    setWindowTitle("New Variant ID Task");
    setWindowFlags(Qt::FramelessWindowHint);
    setFocusPolicy(Qt::ClickFocus);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_TranslucentBackground);

    // Add the dark title bar
    dtb = new DarkTitleBar(this, true);

    // Set the stylesheet for the window
    QFile File(file_paths::STYLESHEET);
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    QFile File2(QApplication::applicationDirPath().append(file_paths::COLORSTYLESHEET).toStdString().c_str());
    File2.open(QFile::ReadOnly);
    QString CStyleSheet = QLatin1String(File2.readAll());
    setStyleSheet(StyleSheet + CStyleSheet);

    // Create layouts
    auto externLayout = new QVBoxLayout();
    externLayout->setContentsMargins(0, 0, 0, 0);
    auto bg = new QFrame(this);
    auto bgLayout = new QVBoxLayout();
    bgLayout->setContentsMargins(0, 0, 0, 0);
    bg->setObjectName("main_window");
    bg->setLayout(bgLayout);
    bgLayout->addWidget(dtb);
    bgLayout->addStretch();
    externLayout->addWidget(bg);
    auto mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(11, 3, 11, 11);
    mainLayout->setSpacing(0);
    // Individual horizontal row layouts
    auto websiteVariantLayout = new QHBoxLayout;
    auto titleSizeLayout = new QHBoxLayout;
    auto startAtLayout = new QHBoxLayout;
    auto profileProxyLayout = new QHBoxLayout;
    auto titleLayout = new QHBoxLayout;

    // Create the widgets
    // WEBSITE & VARIANT ID ROW
    websitesLabel = new QLabel("Website: ", this);
    websitesLabel->setObjectName("addtask_mediocre_text");
    websites = new QComboBox(this);
    websites->addItems(supported_sites::ssStringList);
    variantIDLabel = new QLabel("Variant ID: ");
    variantIDLabel->setObjectName("addtask_mediocre_text");
    variantID = new QLineEdit(this);
    variantID->setObjectName("addtask_editbox");
    variantID->setFixedWidth(135);
    checkForNameButton = new QPushButton("FILL", this);
    checkForNameButton->setObjectName("checkfornamebutton");

    // Add the row to the layout
    websiteVariantLayout->addWidget(websitesLabel);
    websiteVariantLayout->addSpacing(10);
    websiteVariantLayout->addWidget(websites);
    websiteVariantLayout->addSpacing(10);
    websiteVariantLayout->addWidget(variantIDLabel);
    websiteVariantLayout->addWidget(variantID);
    websiteVariantLayout->addSpacing(20);
    websiteVariantLayout->addWidget(checkForNameButton);
    mainLayout->addLayout(websiteVariantLayout);
    mainLayout->addSpacing(5);

    // TITLE ROW WITH COPIES
    variantTitleLabel = new QLabel("Product Title: ", this);
    variantTitleLabel->setObjectName("addtask_mediocre_text");
    variantTitle = new QLineEdit(this);
    variantTitle->setObjectName("addtask_editbox");
    // Add the row to the layout
    titleSizeLayout->addWidget(variantTitleLabel);
    titleSizeLayout->addWidget(variantTitle);
    mainLayout->addLayout(titleSizeLayout);
    mainLayout->addSpacing(10);

    // SIZE & STARTAT ROW
    variantSizeLabel = new QLabel("Size: ", this);
    variantSizeLabel->setObjectName("addtask_mediocre_text");
    variantSizeLabel->setMaximumWidth(35);
    variantSize = new QLineEdit(this);
    variantSize->setObjectName("addtask_editbox");
    variantSize->setMaximumWidth(35);
    startAtLabel = new QLabel("Start at: ", this);
    startAtLabel->setObjectName("addtask_mediocre_text");
    startAtLabel->setMaximumWidth(60);
    startAt = new QDateTimeEdit(this);
    startAt->setObjectName("addtask_datetime");
    startAt->setDisplayFormat("[MMMM d, yyyy] hh:mm:ss");
    // Format the time to minute 00
    QDateTime thecurrent = QDateTime::currentDateTime();
    QTime thetime = thecurrent.time();
    if (thetime.second() != 0) { thetime.setHMS(thetime.hour(), thetime.minute(), 0); }
    thecurrent.setTime(thetime);
    startAt->setDateTime(thecurrent);
    copiesLabel = new QLabel("Copies: ", this);
    copiesLabel->setObjectName("addtask_mediocre_text");
    copiesLabel->setMaximumWidth(45);
    copies = new QLineEdit(this);
    copies->setObjectName("addtask_editbox");
    copies->setValidator(new QIntValidator(0, 20, this));
    copies->setText("1");
    copies->setMaximumWidth(35);
    // Add the row to the layout
    startAtLayout->addStretch();
    startAtLayout->addWidget(variantSizeLabel);
    startAtLayout->addWidget(variantSize);
    startAtLayout->addSpacing(10);
    startAtLayout->addWidget(startAtLabel);
    startAtLayout->addWidget(startAt);
    startAtLayout->addSpacing(10);
    startAtLayout->addWidget(copiesLabel);
    startAtLayout->addWidget(copies);
    startAtLayout->addStretch();
    mainLayout->addLayout(startAtLayout);
    mainLayout->addSpacing(10);

    // PROFILE & PROXY ROW
    profileLabel = new QLabel("Profile: ", this);
    profileLabel->setObjectName("addtask_mediocre_text");
    profileLabel->setFixedWidth(50);
    profile = new QComboBox;
    profile->setFixedWidth(100);
    buildProfilesBox();
    proxyLabel = new QLabel("Proxy: ", this);
    proxyLabel->setObjectName("addtask_mediocre_text");
    proxyLabel->setFixedWidth(45);
    proxy = new QComboBox;
    proxy->setFixedWidth(75);
    buildProxiesBox();
    // Add row to the layout
    profileProxyLayout->addStretch();
    profileProxyLayout->addWidget(profileLabel);
    profileProxyLayout->addWidget(profile);
    profileProxyLayout->addSpacing(10);
    profileProxyLayout->addWidget(proxyLabel);
    profileProxyLayout->addWidget(proxy);
    profileProxyLayout->addStretch();
    mainLayout->addLayout(profileProxyLayout);
    mainLayout->addSpacing(10);

    // TITLE ROW
    titleLabel = new QLabel("Title: ", this);
    titleLabel->setObjectName("task_important_text");
    title = new QLineEdit(this);
    title->setObjectName("task_title_lineedit");
    submit = new QPushButton("SUBMIT", this);
    submit->setObjectName("addtaskbutton");
    submit->setFixedSize(100, 35);
    // Add row to the layout
    titleLayout->addWidget(titleLabel);
    titleLayout->addWidget(title);
    titleLayout->addSpacing(20);
    titleLayout->addWidget(submit);
    mainLayout->addLayout(titleLayout);

    // Set the layout
    bgLayout->addLayout(mainLayout);
    setLayout(externLayout);

    // Connect the submit button to the attemptToSend slot
    connect(submit, SIGNAL(clicked()), this, SLOT(attemptToSend()));
    // Connect the moreinfo button the moreinfodisplay slot
    connect(dtb, SIGNAL(showMIW()), this, SLOT(buildMoreInfoDisplay()));
    // Connect the fill button to the get name size data slot
    connect(checkForNameButton, SIGNAL(clicked()), this, SLOT(fillFromVariant()));
}

// Tries to send the data in the input fields to the main window to build a new task
void AddVIDTaskDisplay::attemptToSend() {

    // Non-required fields: collection, color keywords
    // Check each required input field to make sure a valid task can be built
    if (variantID->text().isEmpty()) { variantID->setFocus(); return; }
    if (variantTitle->text().isEmpty()) { variantTitle->setFocus(); return; }
    if (variantSize->text().isEmpty()) { variantSize->setFocus(); return; }
    if (title->text().isEmpty()) { title->setFocus(); return; }

    // Send the signal with all the data to the main window if all required fields have text in them
    emit sendTask(title->text(), supported_sites::WEBSITES.at(websites->currentText().toStdString()), variantID->text(),
                      variantTitle->text(), variantSize->text(), startAt->dateTime(), profile->currentText(),
                  proxy->currentText(), imageURL, copies->text().toInt());

    // Now close the window because the new task has been made
    close();
}

// Opens the moreinfo display to give a basic tutorial on how to create a new task
void AddVIDTaskDisplay::buildMoreInfoDisplay() {

    // Make sure that a tutorial display is not open already
    if (moreInfoDisplayOpen) { if (mid) { mid->raise(); } return; }

    // Otherwise create a tutorial display and show it
    mid = new MoreInfoDisplay(400, 400, file_paths::MOREINFO_IMG);
    mid->fadeIn();
    mid->setFocus();

    // Connect the close situations
    connect(submit, SIGNAL(clicked()), mid, SLOT(fadeOut()));
    connect(this, SIGNAL(closed()), mid, SLOT(fadeOut()));
    connect(dtb, SIGNAL(hideMIW()), mid, SLOT(fadeOut()));
    // Also notify the add new task window when the window is closed
    connect(mid, SIGNAL(closed()), this, SLOT(MIDClosed()));

    // More info bool set to true
    moreInfoDisplayOpen = true;
}

// Function that simply changes the boolean when the more info display is closed
void AddVIDTaskDisplay::MIDClosed() {
    moreInfoDisplayOpen = false;
}

// Custom close event function that just emits a signal signifying it has closed
void AddVIDTaskDisplay::closeEvent(QCloseEvent *event) {
    // Emit the closed signal and then proceed to cleanup
    emit closed();
    QWidget::closeEvent(event);
}

// Scans the profiles.txt file for the titles of each profile
void AddVIDTaskDisplay::buildProfilesBox() {
    // First add "Random" as an option
    profile->addItem("Random");

    // Open the profiles.txt
    std::ifstream filein(QApplication::applicationDirPath().append(file_paths::PROFILES_TXT).toStdString().c_str());
    std::string tempStr;

    // Cycle through each line and get the profiles' titles
    while (getline(filein, tempStr)) {
        profile->addItem(tempStr.substr(0, tempStr.find(" :-:")).c_str());
    }

    // Close the file in
    filein.close();
}

// Scans the proxies.txt file for the IPs of each proxies
void AddVIDTaskDisplay::buildProxiesBox() {
    // First add "Random" as an option
    proxy->addItem("Random");

    // Open the proxies.txt
    std::ifstream filein(QApplication::applicationDirPath().append(file_paths::PROXIES_TXT).toStdString().c_str());
    std::string tempStr;

    // Cycle through each line and get the proxies' ips
    while (getline(filein, tempStr)) {
        proxy->addItem(tempStr.substr(0, tempStr.find(" :-: ")).c_str());
    }

    // Close the filein
    filein.close();
}

// Fills the add task edits based on the given ID
void AddVIDTaskDisplay::fillFromVariant() {
    // Make sure there is a variant ID
    if (variantID->text().isEmpty()) { variantID->setFocus(); return; }

    // Build a temporary shopify website handler for the given URL and a temporary task ID
    ShopifyWebsiteHandler shopwh(supported_sites::WEBSITES.at(websites->currentText().toStdString()), "temp");

    // Get the tuple of the necessary information
    std::tuple<std::string, std::string, std::string> dataPack = shopwh.getNameSizeImage(variantID->text().toStdString());

    // Finally set the current texts for each lineedit to their respective data
    variantTitle->setText(std::get<0>(dataPack).c_str());
    variantTitle->setCursorPosition(0);
    variantSize->setText(std::get<1>(dataPack).c_str());
    variantSize->setCursorPosition(0);

    // Get the image URl from the tuple too
    imageURL = std::get<2>(dataPack).c_str();
}

// MORE INFO DISPLAY ClASS
// Constructor to build the pop up window
MoreInfoDisplay::MoreInfoDisplay(unsigned int width, unsigned int height, const char *img_filepath,
                                      QWidget *parent) : QWidget(parent) {
    // Fixed size of the window
    setFixedSize(width + 22, height + 22);
    setWindowTitle("More Info");
    setWindowFlags(Qt::FramelessWindowHint);
    setObjectName("moreinfo_window");
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_TranslucentBackground);

    // Set the stylesheet for the window
    QFile File(file_paths::STYLESHEET);
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    QFile File2(QApplication::applicationDirPath().append(file_paths::COLORSTYLESHEET).toStdString().c_str());
    File2.open(QFile::ReadOnly);
    QString CStyleSheet = QLatin1String(File2.readAll());
    setStyleSheet(StyleSheet + CStyleSheet);

    // Create the window layouts
    auto externLayout = new QVBoxLayout();
    auto mainLayout = new QVBoxLayout();

    // Build the QFrame
    background = new QFrame(this);
    background->setObjectName("moreinfo_window");
    background->setFixedSize(width, height);

    // Create the image for the QLabel to be displayed in the window and set it to the QLabel
    imageLabel = new QLabel(background);
    QPixmap img = QPixmap::fromImage(QImage(img_filepath).scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageLabel->setPixmap(img);
    mainLayout->addWidget(imageLabel);

    // Set the window's layout
    background->setLayout(mainLayout);
    externLayout->addWidget(background);
    setLayout(externLayout);
}

// Custom close event which emits the closed() signal and then does normal close procedures
void MoreInfoDisplay::closeEvent(QCloseEvent *event) {
    emit closed();
    QWidget::closeEvent(event);
}

// Fades the window in and shows it
void MoreInfoDisplay::fadeIn() {
    auto eff = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(eff);
    auto a = new QPropertyAnimation(eff, "opacity");
    a->setDuration(250);
    a->setStartValue(0);
    a->setEndValue(1);
    a->setEasingCurve(QEasingCurve::InOutQuad);
    a->start(QPropertyAnimation::DeleteWhenStopped);
    show();
}

// Fades the window out and closes it
void MoreInfoDisplay::fadeOut() {
    auto eff = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(eff);
    auto a = new QPropertyAnimation(eff, "opacity");
    a->setDuration(250);
    a->setStartValue(1);
    a->setEndValue(0);
    a->setEasingCurve(QEasingCurve::InOutQuad);
    a->start(QPropertyAnimation::DeleteWhenStopped);
    connect(a, SIGNAL(finished()), this, SLOT(close()));
}