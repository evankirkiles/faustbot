//
// Created by Evan Kirkiles on 7/21/18.
//

#include "window.hpp"

// Constructor for the Bot's main window
BotWindow::BotWindow(QWidget *parent) : QWidget(parent) {
    // Set the size of the window
    setFixedSize(1000, 600);
    // Also set the stylesheet for the window
    QFile File("./shopifybot/Graphics/stylesheet.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    setStyleSheet(StyleSheet);
    // Finally, set some various window attributes
    setAttribute(Qt::WA_QuitOnClose);
    setWindowTitle(tr("Shopify Bot"));
    setObjectName("main_window");

    // SIZE POLICIES
    // Size policies for top row
    QSizePolicy spRight(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spRight.setHorizontalStretch(3);

    // LAYOUTS
    // Create two vertical layouts within a horizontal main layout
    auto mainLayout = new QVBoxLayout;
    auto topLayout = new QHBoxLayout;
    auto botLayout = new QHBoxLayout;
    // Vertical layouts for the top layout
    auto leftColumn = new QVBoxLayout;
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
    setLayout(mainLayout);

    // WIDGETS
    // Left column widgets
    QImage img(file_paths::LOGO_FILE);
    QPixmap logoimg;
    logoimg = logoimg.fromImage(img).scaled(420, 400, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    logoimg.setDevicePixelRatio(2.0);
    logo = new QLabel();
    logo->setPixmap(logoimg);
    title = new QLabel("SHOPIFY BOT", this);
    title->setAlignment(Qt::AlignCenter);
    title->setObjectName("bot_title");
    billing = new QPushButton("Billing Info", this);
    billing->setObjectName("sidebuttons");
    proxies = new QPushButton("Proxies", this);
    proxies->setObjectName("sidebuttons");
    logs = new QPushButton("Logs", this);
    logs->setObjectName("sidebuttons");
    about = new QPushButton("About", this);
    about->setObjectName("sidebuttons");
    // Add widgets to the left column
    leftColumn->addWidget(logo);
    leftColumn->addWidget(title);
    leftColumn->addWidget(billing);
    leftColumn->addWidget(proxies);
    leftColumn->addWidget(logs);
    leftColumn->addWidget(about);
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

    // Add a task
    addTask("Kith Task", supported_sites::KITH, "1", "/collections/footwear", {"Asics"}, {"White"}, "200", 3, 3);
    addTask("Social Status Task", supported_sites::SOCIALSTATUS, "1", "/collections/sneakers", {"Revaderchi"},
            {"Black", "Granite"}, "9", 2, 3);

    // Add a task when button is clicked
    connect(about, SIGNAL(clicked(bool)), this, SLOT(testtask()));
}

// Creates a task and adds it to the tasklist
void BotWindow::addTask(const std::string &title, const URLAndMethod &website, const std::string &identifier,
                        const std::string &collection, const std::vector<std::string> &keywords,
                        const std::vector<std::string> &colorKeywords, const std::string &size,
                        const unsigned int quantity, unsigned int resultsToCheck, unsigned int frequency) {

    // Create a new task
    auto newtask = new TaskWidget(title, website, identifier, collection, keywords, colorKeywords, size,
                                        quantity, resultsToCheck, frequency, tasklistwidget);

    // Adds the task to the qvboxlayout
    tasklistLayout->addWidget(newtask);
    // Show the new task
    newtask->show();
}

// Adds a task
void BotWindow::testtask() {
    addTask("Bodega Task", supported_sites::BODEGA, "1", "/collections/footwear", {"Air Max"}, {"Cargo"}, "9", 1, 3);
}