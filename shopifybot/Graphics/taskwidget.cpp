//
// Created by Evan Kirkiles on 7/22/18.
//

#include "taskwidget.hpp"

// Constructor for building the task
TaskWidget::TaskWidget(const std::string& p_title, const URLAndMethod& p_website, const std::string& p_identifier,
                       const std::string& p_collection, const std::vector<std::string>& p_keywords,
                       const std::vector<std::string>& p_colorKeywords, const std::string& p_size,
                       const unsigned int p_quantity, bool* p_logWindowOpen, unsigned int p_resultsToCheck,
                       unsigned int p_frequency, QWidget *parent) :
                               task(p_title, p_website, p_identifier, p_collection, p_keywords, p_colorKeywords,
                               p_size, p_quantity, p_resultsToCheck, p_frequency),
                               title(new QLabel(p_title.c_str(), this)),
                               website(new QLabel(p_website.baseURL, this)),
                               collection(new QLabel(p_collection.c_str(), this)),
                               size(new QLabel(p_size.c_str(), this)),
                               quantity(new QLabel(std::to_string(p_quantity).c_str(), this)),
                               logWindowOpen(p_logWindowOpen),
                               QFrame(parent) {

    // Ensure the widget deletes itself when it is closed
    setAttribute(Qt::WA_DeleteOnClose);

    // Initialize the keywords combo boxes
    std::string placeholderkeywords;
    for (const std::string& kwd : p_keywords) { placeholderkeywords.append(kwd + ", "); }
    keywords = new QTextEdit(placeholderkeywords.c_str(), this);
    keywords->setReadOnly(true);
    keywords->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    keywords->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    keywords->setFixedWidth(100);
    placeholderkeywords = "";
    for (const std::string& kwd : p_colorKeywords) { placeholderkeywords.append(kwd + ", "); }
    colorKeywords = new QTextEdit(placeholderkeywords.c_str(), this);
    colorKeywords->setReadOnly(true);
    colorKeywords->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    colorKeywords->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    colorKeywords->setFixedWidth(100);

    // First, set the style sheet of the frame
    setObjectName("task");

    // Stylesheet settings for the different labels
    title->setObjectName("task_important_text");
    title->setFixedWidth(150);
    website->setObjectName("task_mediocre_text");
    website->setMaximumWidth(150);
    collection->setObjectName("task_mediocre_text");
    collection->setMaximumWidth(150);
    size->setObjectName("task_important_var");
    quantity->setObjectName("task_important_var");

    // Create the layouts for the QLabels
    // Vertical layouts
    auto firstcol = new QVBoxLayout();
    auto secondcol = new QVBoxLayout();
    auto thirdcol = new QVBoxLayout();
    auto fourthcol = new QVBoxLayout();
    auto fifthcol = new QVBoxLayout();
    fifthcol->setAlignment(Qt::AlignHCenter);
    // Main horizontal layout
    auto row = new QHBoxLayout();
    // Sub horizontal layouts
    auto keywordHor = new QHBoxLayout();
    auto keywordtitle = new QLabel("Keywords: ", this);
    keywordtitle->setObjectName("task_mediocre_title");
    keywordHor->addWidget(keywordtitle);
    keywordHor->addWidget(keywords);
    auto colorKeywordHor = new QHBoxLayout();
    auto colorKeywordtitle = new QLabel("Color Keywords: ", this);
    colorKeywordtitle->setObjectName("task_mediocre_title");
    colorKeywordHor->addWidget(colorKeywordtitle);
    colorKeywordHor->addWidget(colorKeywords);
    auto quantityHor = new QHBoxLayout();
    auto quantitytitle = new QLabel("Quantity: ", this);
    quantitytitle->setObjectName("task_mediocre_title_v2");
    quantityHor->addWidget(quantitytitle);
    quantityHor->addWidget(quantity);
    auto sizeHor = new QHBoxLayout();
    auto sizetitle = new QLabel("Size: ", this);
    sizetitle->setObjectName("task_mediocre_title_v2");
    sizeHor->addWidget(sizetitle);
    sizeHor->addWidget(size);
    auto statusHor = new QHBoxLayout();

    // Separator line between first column and second column
    separator1 = new QFrame(this);
    separator1->setFrameShape(QFrame::VLine);
    separator1->setFrameShadow(QFrame::Sunken);
    separator1->setObjectName("vertical_line");

    // Separator line between third column and fourth column
    separator2 = new QFrame(this);
    separator2->setFrameShape(QFrame::VLine);
    separator2->setFrameShadow(QFrame::Sunken);
    separator2->setObjectName("vertical_line");

    // Build the play button
    play = new ClickableCheckableImage(80, 100, file_paths::PLAY2_IMG, file_paths::PLAY_IMG,
            file_paths::STOP2_IMG, file_paths::STOP_IMG, file_paths::PLAY_DISBL_IMG, this);
    // Build the edit button
    edit = new ClickableImage(100, 100, file_paths::EDIT2_IMG, file_paths::EDIT_IMG, this);

    // Builds the status identifier
    auto statusFixed = new QLabel("STATUS:", this);
    statusFixed->setObjectName("status_title");
    status = new QTextEdit("Idle", this);
    status->setObjectName("status_text");
    status->setReadOnly(true);
    status->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    status->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    status->setAlignment(Qt::AlignHCenter);
    status->setFixedWidth(100);
    logsButton = new ClickableImage(80, 80, file_paths::LOGS2_IMG, file_paths::LOGS_IMG, this);
    statusHor->addWidget(statusFixed);
    statusHor->addWidget(logsButton);

    // Delete button
    deleteButton = new ClickableImage(60, 60, file_paths::DELETE2_IMG, file_paths::DELETE_IMG, this);

    // Add the labels and layouts to the main row
    firstcol->addWidget(title);
    firstcol->addWidget(website);
    firstcol->addWidget(collection);
    row->addLayout(firstcol);
    row->addWidget(separator1);
    secondcol->addLayout(keywordHor);
    secondcol->addLayout(colorKeywordHor);
    row->addLayout(secondcol);
    thirdcol->addLayout(sizeHor);
    thirdcol->addLayout(quantityHor);
    row->addLayout(thirdcol);
    row->addWidget(separator2);
    fourthcol->addWidget(play);
    fourthcol->addWidget(edit);
    row->addLayout(fourthcol);
    fifthcol->addLayout(statusHor);
    fifthcol->addWidget(status);
    row->addLayout(fifthcol);
    row->addWidget(deleteButton);

    // Set the qframe's layout to the row
    setLayout(row);

    // Connect clicking play to beginning the task.
    connect(play, SIGNAL(runTask()), this, SLOT(run()));
    // Connect the close button to deleting the task as well
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(exit()));

    // Connect the log button to showing the logfile window
    connect(logsButton, SIGNAL(clicked()), this, SLOT(showLogs()));
}

// Tells the event loop to run the task
void TaskWidget::run() {

    // Initializes the thread and moves the task onto it
    taskthread = new QThread;
    auto temptask = new Task(task.title, task.swh.sourceURL, task.swh.taskID, task.collection, task.keywords,
            task.colorKeywords, task.size, task.quantity, task.resultsToCheck, task.frequency);
    temptask->moveToThread(taskthread);

    // Remove all connections to play so do not set multiple shouldcontinues
    play->disconnect();
    connect(play, SIGNAL(runTask()), this, SLOT(run()));

    // Connects all the necessary signals and slots for communication between the two
    connect(taskthread, SIGNAL(started()), temptask, SLOT(run()));
    connect(play, &ClickableCheckableImage::interrupt, [this, temptask] () {
        play->disable();
        setStatus("Stopping...", "#e26c6c");
        temptask->shouldcontinue = false;});
    connect(temptask, SIGNAL(finished()), play, SLOT(enable()));
    connect(temptask, SIGNAL(finished()), taskthread, SLOT(quit()));
    connect(temptask, SIGNAL(finished()), temptask, SLOT(deleteLater()));
    connect(temptask, SIGNAL(finished()), taskthread, SLOT(deleteLater()));

    // Also connect the status signal
    connect(temptask, SIGNAL(status(QString, QString)), this, SLOT(setStatus(QString, QString)));

    // Begin the thread!
    taskthread->start();
}

// Sets the text in the status textedit
void TaskWidget::setStatus(QString text, QString hexColor) {

    // Simply change the text in the status bar to the given text with the given color
    status->setText(text);
    status->setStyleSheet(std::string("color: ").append(hexColor.toStdString()).append(";").c_str());

    // If task has actually finished, change the play button to the replay button
    if (text == "Finished!") {
        play->changeCheckedImg(file_paths::REPLAY2_IMG, file_paths::REPLAY_IMG);
        play->isChecked = false;
    }
}

// Deletes the widget as long as no task is currently running
void TaskWidget::exit() {

    // Check if task is running by seeing what state the play button is
    if (play->isChecked) {
        return;
    } else {
        // Delete the log file when this task is deleted
        remove(std::string(file_paths::TASK_LOG).append("task_logs_").append(task.swh.sourceURL.title).append(task.swh.taskID).append(".txt").c_str());
        this->close();
    }
}

// Create a logs window and display it
void TaskWidget::showLogs() {
    // Make sure a log window is not already open
    if (*logWindowOpen) { if (lfd) { lfd->raise(); } return; }

    // Otherwise create a new log window and show it
    lfd = new LogFileDisplay(task.title, std::string(file_paths::TASK_LOG).append("task_logs_").append(task.swh.sourceURL.title).append(task.swh.taskID).append(".txt"));
    lfd->show();
    lfd->setFocus();
    // Connect the closeLogs function of lfd to the delete button of the task
    connect(deleteButton, SIGNAL(clicked()), lfd, SLOT(close()));
    connect(lfd, SIGNAL(closed()), this, SLOT(logsClosed()));
    // Notify the main window that a log window is open
    *logWindowOpen = true;
}

// Called when the logFile window is closed
void TaskWidget::logsClosed() {
    *logWindowOpen = false;
}