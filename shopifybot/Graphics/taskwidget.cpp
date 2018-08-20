//
// Created by Evan Kirkiles on 7/22/18.
//

#include "taskwidget.hpp"

// Constructor for building the task
TaskWidget::TaskWidget(const std::string& p_title, const URLAndMethod& p_website, const std::string& p_identifier,
                       const std::string& p_collection, const std::vector<std::string>& p_keywords,
                       const std::vector<std::string>& p_colorKeywords, const std::string& p_size,
                       const QDateTime& p_startDate, const std::string& p_profile, const std::string& p_proxy,
                       bool* p_logWindowOpen, bool* p_editWindowOpen, unsigned int p_resultsToCheck,
                       unsigned int p_frequency, QWidget *parent) :
                               task(new Task(p_title, p_website, p_identifier, p_collection, p_keywords, p_colorKeywords,
                               p_size, p_startDate, p_profile, p_proxy, p_resultsToCheck, p_frequency)),
                               identifier(new QLabel(p_identifier.c_str(), this)),
                               title(new QLabel(p_title.c_str(), this)),
                               website(new QLabel(p_website.baseURL, this)),
                               collection(new QLabel(p_collection.c_str(), this)),
                               size(new QLabel(p_size.c_str(), this)),
                               startAt(new QDateTimeEdit(this)),
                               editWindowOpen(p_editWindowOpen),
                               logWindowOpen(p_logWindowOpen),
                               taskfreq(p_frequency),
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
    identifier->setObjectName("task_identifier");
    identifier->setContentsMargins(0, 0, 0, 0);
    identifier->setAlignment(Qt::AlignCenter);
    identifier->setFixedWidth(20);
    title->setObjectName("task_important_text");
    title->setMaximumWidth(250);
    title->setMinimumWidth(150);
    website->setObjectName("task_mediocre_text");
    website->setMaximumWidth(150);
    collection->setObjectName("task_mediocre_text");
    collection->setMaximumWidth(150);
    size->setObjectName("task_important_var");
    startAt->setObjectName("task_important_var");

    // Create the layouts for the QLabels
    // Vertical layouts
    auto zerocol = new QHBoxLayout();
    zerocol->setSpacing(0);
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
    auto startAtTitle = new QLabel("START AT: ", this);
    startAtTitle->setObjectName("task_tiny_text");
    startAt->setObjectName("task_dateedit");
    startAt->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    startAt->setDateTime(task->startat);
    startAt->setReadOnly(true);
    auto sizeHor = new QHBoxLayout();
    auto sizetitle = new QLabel("Size: ", this);
    sizetitle->setObjectName("task_mediocre_title_v2");
    size->setMinimumWidth(50);
    sizeHor->addWidget(sizetitle);
    sizeHor->addWidget(size);
    auto statusHor = new QHBoxLayout();

    // Separator line between identifier and first column
    separator0 = new QFrame(this);
    separator0->setFrameShape(QFrame::VLine);
    separator0->setFrameShadow(QFrame::Sunken);
    separator0->setContentsMargins(0, 0, 0, 0);
    separator0->setObjectName("vertical_line");

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
    play = new ClickableCheckableImage(80, 100, 4, file_paths::PLAY2_IMG, file_paths::PLAY_IMG,
            file_paths::STOP2_IMG, file_paths::STOP_IMG, file_paths::PLAY_DISBL_IMG, this);
    // Build the edit button
    edit = new ClickableImage(100, 100, 4, file_paths::EDIT2_IMG, file_paths::EDIT_IMG, this);

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
    logsButton = new ClickableImage(80, 80, 4, file_paths::LOGS2_IMG, file_paths::LOGS_IMG, this);
    statusHor->addWidget(statusFixed);
    statusHor->addWidget(logsButton);

    // Delete button
    deleteButton = new ClickableImage(60, 60, 4, file_paths::DELETE2_IMG, file_paths::DELETE_IMG, this);

    // Add the labels and layouts to the main row
    zerocol->addWidget(identifier);
    zerocol->addWidget(separator0);
    row->addLayout(zerocol);
    firstcol->addWidget(title);
    firstcol->addWidget(website);
    firstcol->addWidget(collection);
    row->addLayout(firstcol);
    row->addStretch();
    row->addWidget(separator1);
    secondcol->addLayout(keywordHor);
    secondcol->addLayout(colorKeywordHor);
    row->addLayout(secondcol);
    thirdcol->addLayout(sizeHor);
    thirdcol->addWidget(startAtTitle);
    thirdcol->addWidget(startAt);
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
    row->setContentsMargins(0, 11, 11, 11);

    // Connect clicking play to beginning the task.
    connect(play, SIGNAL(runTask()), this, SLOT(run()));
    // Connect the close button to deleting the task as well
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(exit()));

    // Connect the log button to showing the logfile window
    connect(logsButton, SIGNAL(clicked()), this, SLOT(showLogs()));
    // Connect the edit button to showing the edit window
    connect(edit, SIGNAL(clicked()), this, SLOT(showEdit()));
}

// Tells the event loop to run the task
void TaskWidget::run() {

    // Make sure the play button changes if it is called not by the play button
    // If the play button is not available when run is picked, then do nothing
    if (play->isChecked) {
        return;
    }
    play->clickedPlay();

    // Add a small delay to prevent strange error code
    std::this_thread::sleep_for(std::chrono::nanoseconds(10000000));

    // Initializes the thread and moves a copy of the task onto it
    taskthread = new QThread;
    auto temptask = new Task(task->title, task->swh.sourceURL, task->swh.taskID, task->collection, task->keywords,
            task->colorKeywords, task->size, task->startat, task->profile, task->proxy, task->resultsToCheck, task->frequency);
    temptask->moveToThread(taskthread);

    // Remove all connections to play so do not set multiple shouldcontinues
    play->disconnect();
    connect(play, SIGNAL(runTask()), this, SLOT(run()));

    // Connects all the necessary signals and slots for communication between the two
    connect(taskthread, SIGNAL(started()), temptask, SLOT(run()));
    connect(play, SIGNAL(interrupt()), this, SLOT(stopWidget()));
    connect(this, &TaskWidget::stopTask, [temptask] () {
        temptask->shouldcontinue = false;
    });
    connect(temptask, SIGNAL(finished()), play, SLOT(enable()));
    connect(temptask, SIGNAL(finished()), taskthread, SLOT(quit()));
    connect(temptask, SIGNAL(finished()), temptask, SLOT(deleteLater()));
    connect(temptask, SIGNAL(finished()), taskthread, SLOT(deleteLater()));

    // Also connect the status signal
    connect(temptask, SIGNAL(status(QString, QString)), this, SLOT(setStatus(QString, QString)));

    // Begin the thread!
    taskthread->start();
}

// Checks the time against the taskwidget's start time. If there is a match (or if early by a second), then run the task.
void TaskWidget::checkTime(QDateTime time) {
    // Make sure the task still exists first
    if (deleted) { return; }
    // Check the difference between times, making sure it is within a second of accuracy
    if (time.secsTo(task->startat) < 1 && time.secsTo(task->startat) > -1) {
        run();
    }
}

// Tells the task to interrupt after the loop has finished
void TaskWidget::stopWidget() {
    if (play->isChecked) {
        play->disable();
        setStatus("Stopping...", "#e26c6c");
        emit stopTask();
    }
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
    } else if (text == "Interrupted.") {
        play->isChecked = false;
    }
}

// Deletes the widget as long as no task is currently running
void TaskWidget::exit() {

    // Check if task is running by seeing what state the play button is
    if (play->isChecked || !play->enabled) {
        return;
    } else {
        // Delete the log file when this task is deleted
        remove(std::string(file_paths::TASK_LOG).append("task_logs_").append(task->swh.sourceURL.title).append(task->swh.taskID).append(".txt").c_str());
        // Remove all connections
        deleted = true;
        this->disconnect();
        this->close();
    }
}

// Create a logs window and display it
void TaskWidget::showLogs() {
    // Make sure a log window is not already open
    if (*logWindowOpen) { if (lfd) { lfd->raise(); } return; }

    // Otherwise create a new log window and show it
    lfd = new LogFileDisplay(task->title, std::string(file_paths::TASK_LOG).append("task_logs_").append(task->swh.sourceURL.title).append(task->swh.taskID).append(".txt"));
    lfd->show();
    lfd->setFocus();
    // Connect the closeLogs function of lfd to the delete button of the task
    connect(deleteButton, SIGNAL(clicked()), lfd, SLOT(close()));
    connect(lfd, SIGNAL(closed()), this, SLOT(logsClosed()));
    // Notify the main window that a log window is open
    *logWindowOpen = true;
}

// Create an edit window and display it
void TaskWidget::showEdit() {
    // Make sure a log window is not already open
    if (*editWindowOpen) { if (etd) { etd->raise(); } return; }

    // Otherwise create a new edit window and show it
    etd = new EditTaskDisplay(title->text(), supported_sites::WEBSITES_BWD.at(website->text().toStdString()).c_str(),
                              collection->text(), keywords->toPlainText(), colorKeywords->toPlainText(), size->text(),
                              startAt->dateTime(), task->profile.c_str(), task->proxy.c_str());
    etd->show();
    etd->setFocus();
    // Connect the closeLogs function of lfd to the delete button of the task
    connect(deleteButton, SIGNAL(clicked()), etd, SLOT(close()));
    connect(etd, SIGNAL(closed()), this, SLOT(editClosed()));

    // Finally connect the task edited signal of the display to the receiving end of the taskwidget
    connect(etd, SIGNAL(sendTaskEdit(QString, URLAndMethod, QString, QString, QString, QString, QDateTime, QString, QString)),
            this, SLOT(acceptTaskEdit(QString, URLAndMethod, QString, QString, QString, QString, QDateTime, QString, QString)));

    // Notify the main window that a log window is open
    *editWindowOpen = true;
}

// Receives a packet of information and rebuilds the task and the task widget with edited info
void TaskWidget::acceptTaskEdit(QString p_title, URLAndMethod p_website, QString p_collection, QString p_keywords,
                                QString p_colorKeywords, QString p_size, QDateTime p_start, QString p_profile,
                                QString p_proxy, unsigned int frequency) {

    // Rename the old logs file to the new one
    std::rename(std::string(file_paths::TASK_LOG).append("task_logs_").append(task->swh.sourceURL.title).append(task->swh.taskID).append(".txt").c_str(),
                std::string(file_paths::TASK_LOG).append("task_logs_").append(p_website.title).append(task->swh.taskID).append(".txt").c_str());

    // First, change the visuals of the task widget to match the new task
    title->setText(p_title);
    website->setText(p_website.baseURL);
    collection->setText(p_collection);
    keywords->setText(p_keywords);
    colorKeywords->setText(p_colorKeywords);
    size->setText(p_size);
    startAt->setDateTime(p_start);
    taskfreq = frequency;

    // Then, delete the current task and rebuild it with the edited task
    task = new Task(p_title.toStdString(), p_website, task->swh.taskID, p_collection.toStdString(),
            vectorFromString(p_keywords.toStdString()), vectorFromString(p_colorKeywords.toStdString()),
                    p_size.toStdString(), p_start, p_profile.toStdString(), p_proxy.toStdString(), frequency);\

    // If a log window was open, then close it
    if (logWindowOpen) { lfd->close(); }
}

// Called when the edit task window is closed
void TaskWidget::editClosed() {
    *editWindowOpen = false;
}

// Called when the logFile window is closed
void TaskWidget::logsClosed() {
    *logWindowOpen = false;
}

// VARIANT ID TASK CLASS
// Constructor that initializes the variant ID task widget
VIDTaskWidget::VIDTaskWidget(const std::string &p_title, const URLAndMethod &p_website, const std::string& p_identifier,
                             const std::string &p_variantID, const std::string &p_profile, const std::string &p_proxy,
                             const QDateTime &p_startAt, const std::string &prodTitle, const std::string& prodSize,
                             bool* p_logWindowOpen, bool* p_editWindowOpen, unsigned int frequency, QWidget *parent) :
        title(new QLabel(p_title.c_str(), this)),
        task(new VariantIDTask(p_title, p_website, p_identifier, p_variantID, p_startAt, p_profile, p_proxy, frequency)),
        identifier(new QLabel(p_identifier.c_str(), this)),
        website(new QLabel(p_website.baseURL, this)),
        variantId(new QLabel(p_variantID.c_str(), this)),
        variantName(new QTextEdit(prodTitle.c_str(), this)),
        variantSize(new QLabel(prodSize.c_str(), this)),
        startAt(new QDateTimeEdit(this)),
        editWindowOpen(p_editWindowOpen),
        logWindowOpen(p_logWindowOpen) {

    // Ensure the widget deletes itself when it is closed
    setAttribute(Qt::WA_DeleteOnClose);

    // Set the style sheet of the frame
    setObjectName("task");

    // Builds the textedit box
    variantName->setReadOnly(true);
    variantName->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    variantName->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    variantName->setFixedWidth(204);

    // Stylesheet settings for the different labels
    identifier->setObjectName("task_identifier");
    identifier->setContentsMargins(0, 0, 0, 0);
    identifier->setAlignment(Qt::AlignCenter);
    identifier->setFixedWidth(20);
    title->setObjectName("task_important_text");
    title->setMaximumWidth(350);
    title->setMinimumWidth(150);
    website->setObjectName("task_mediocre_text");
    website->setMaximumWidth(150);
    variantId->setObjectName("task_mediocre_text");
    variantId->setMaximumWidth(150);
    variantSize->setObjectName("task_important_var");
    startAt->setObjectName("task_important_var");

    // Create the layouts for the QLabels
    // Vertical layouts
    auto zerocol = new QHBoxLayout();
    zerocol->setSpacing(0);
    auto firstcol = new QVBoxLayout();
    auto secondcol = new QVBoxLayout();
    auto thirdcol = new QVBoxLayout();
    auto fourthcol = new QVBoxLayout();
    auto fifthcol = new QVBoxLayout();
    fifthcol->setAlignment(Qt::AlignHCenter);
    // Main horizontal layouts
    auto row = new QHBoxLayout();
    // Sub horizontal layouts
    auto variantTitleLabel = new QLabel("Product Title: ", this);
    variantTitleLabel->setObjectName("task_mediocre_title_v2");
    variantTitleLabel->setFixedWidth(75);
    auto startAtTitle = new QLabel("START AT: ", this);
    startAtTitle->setObjectName("task_tiny_text");
    startAt->setObjectName("task_dateedit");
    startAt->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    startAt->setDateTime(task->startat);
    startAt->setReadOnly(true);
    auto sizeLayout = new QHBoxLayout();
    auto sizeLabel = new QLabel("Size: ", this);
    sizeLabel->setObjectName("task_mediocre_title_v2");
    variantSize->setMinimumWidth(50);
    sizeLayout->addWidget(sizeLabel);
    sizeLayout->addWidget(variantSize);
    auto statusHor = new QHBoxLayout();

    // Separator line between identifier and first column
    separator0 = new QFrame(this);
    separator0->setFrameShape(QFrame::VLine);
    separator0->setFrameShadow(QFrame::Sunken);
    separator0->setContentsMargins(0, 0, 0, 0);
    separator0->setObjectName("vertical_line");

    // Separators in between first column and second column
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
    play = new ClickableCheckableImage(80, 100, 4, file_paths::PLAY2_IMG, file_paths::PLAY_IMG,
                                       file_paths::STOP2_IMG, file_paths::STOP_IMG, file_paths::PLAY_DISBL_IMG, this);
    // Build the edit button
    edit = new ClickableImage(100, 100, 4, file_paths::EDIT2_IMG, file_paths::EDIT_IMG, this);

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
    logsButton = new ClickableImage(80, 80, 4, file_paths::LOGS2_IMG, file_paths::LOGS_IMG, this);
    statusHor->addWidget(statusFixed);
    statusHor->addWidget(logsButton);

    // Delete button
    deleteButton = new ClickableImage(60, 60, 4, file_paths::DELETE2_IMG, file_paths::DELETE_IMG, this);

    // Add the labels and layouts to the main row
    zerocol->addWidget(identifier);
    zerocol->addWidget(separator0);
    row->addLayout(zerocol);
    firstcol->addWidget(title);
    firstcol->addWidget(website);
    firstcol->addWidget(variantId);
    row->addLayout(firstcol);
    row->addStretch();
    row->addWidget(separator1);
    secondcol->addWidget(variantTitleLabel);
    secondcol->addWidget(variantName);
    row->addLayout(secondcol);
    thirdcol->addLayout(sizeLayout);
    thirdcol->addWidget(startAtTitle);
    thirdcol->addWidget(startAt);
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
    row->setContentsMargins(0, 11, 11, 11);

    // Connect clicking play to beginning the task.
    connect(play, SIGNAL(runTask()), this, SLOT(run()));
    // Connect the close button to deleting the task as well
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(exit()));

    // Connect the log button to showing the logfile window
    connect(logsButton, SIGNAL(clicked()), this, SLOT(showLogs()));
    // Connect the edit button to showing the edit window
    connect(edit, SIGNAL(clicked()), this, SLOT(showEdit()));
}

// Tells the event loop to run the task
void VIDTaskWidget::run() {

    // Make sure the play button changes if it is called not by the play button
    // If the play button is not available when run is called, then do nothing
    if (play->isChecked) { return; }
    play->clickedPlay();

    // Add a small delay to prevent strange error code
    std::this_thread::sleep_for(std::chrono::nanoseconds(10000000));

    // Initializes the thread and moves a copy of the task onto it
    taskthread = new QThread;
    auto temptask = new VariantIDTask(task->title, task->swh.sourceURL, task->swh.taskID, task->variantID, task->startat,
            task->profile, task->proxy, task->frequency);
    temptask->moveToThread(taskthread);

    // Remove all connections to play so do not set multiple shouldcontinues
    play->disconnect();
    connect(play, SIGNAL(runTask()), this, SLOT(run()));

    // Connects all the necessary signals and slots for communication between the two
    connect(taskthread, SIGNAL(started()), temptask, SLOT(run()));
    connect(play, SIGNAL(interrupt()), this, SLOT(stopWidget()));
    connect(this, &VIDTaskWidget::stopTask, [temptask] () {
        temptask->shouldcontinue = false;
    });
    connect(temptask, SIGNAL(finished()), play, SLOT(enable()));
    connect(temptask, SIGNAL(finished()), taskthread, SLOT(quit()));
    connect(temptask, SIGNAL(finished()), temptask, SLOT(deleteLater()));
    connect(temptask, SIGNAL(finished()), taskthread, SLOT(deleteLater()));

    // Also connect the status signal
    connect(temptask, SIGNAL(status(QString, QString)), this, SLOT(setStatus(QString, QString)));

    // Begin the thread!
    taskthread->start();
}

// Checks the time against the vidtaskwidget's start time. If there is a match (or if early by a second), then run the task.
void VIDTaskWidget::checkTime(QDateTime time) {
    // Make sure that the task actually still exists so no error crops up
    if (deleted) { return; }
    // Check the difference between times, making sure it is within a second of accuracy
    if (time.secsTo(task->startat) < 1 && time.secsTo(task->startat) > -1) {
        run();
    }
}

// Tells the task to interrupr after the loop has finished
void VIDTaskWidget::stopWidget() {
    if (play->isChecked) {
        play->disable();
        setStatus("Stopping...", "#e26c6c");
        emit stopTask();
    }
}

// Sets the text in the status textedit
void VIDTaskWidget::setStatus(QString text, QString hexColor) {

    // Simply change the text in the status bar to the given text with the given color
    status->setText(text);
    status->setStyleSheet(std::string("color: ").append(hexColor.toStdString()).append(";").c_str());

    // If task has actually finished, change the play button to the replay button
    if (text == "Finished!") {
        play->changeCheckedImg(file_paths::REPLAY2_IMG, file_paths::REPLAY_IMG);
        play->isChecked = false;
    } else if (text == "Interrupted.") {
        play->isChecked = false;
    }
}

// Deletes the widget as long as no task is currently running
void VIDTaskWidget::exit() {

    // Check if task is running by seeing what state the play button is
    if (play->isChecked || !play->enabled) {
        return;
    } else {
        // Delete the log file when this task is deleted
        remove(std::string(file_paths::TASK_LOG).append("task_logs_").append(task->swh.sourceURL.title).append(task->swh.taskID).append(".txt").c_str());
        // Remove all connections
        deleted = true;
        this->disconnect();
        this->close();
    }
}

// Create a logs window and display it
void VIDTaskWidget::showLogs() {
    // Make sure a log window is not already open
    if (*logWindowOpen) { if (lfd) { lfd->raise(); } return; }

    // Otherwise create a new log window and show it
    lfd = new LogFileDisplay(task->title, std::string(file_paths::TASK_LOG).append("task_logs_").append(task->swh.sourceURL.title).append(task->swh.taskID).append(".txt"));
    lfd->show();
    lfd->setFocus();
    // Connect the closeLogs function of lfd to the delete button of the task
    connect(deleteButton, SIGNAL(clicked()), lfd, SLOT(close()));
    connect(lfd, SIGNAL(closed()), this, SLOT(logsClosed()));
    // Notify the main window that a log window is open
    *logWindowOpen = true;
}

// TODO: Make frequency editable in edit window and in new task window
// Create an edit window and display it
void VIDTaskWidget::showEdit() {
    // Make sure a log window is not already open
    if (*editWindowOpen) { if (etd) { etd->raise(); } return; }

    // Otherwise create a new edit window and show it
    etd = new VIDTaskEditDisplay(title->text(), supported_sites::WEBSITES_BWD.at(website->text().toStdString()).c_str(),
            variantId->text(), variantName->toPlainText(), variantSize->text(), startAt->dateTime(), task->profile.c_str(), task->proxy.c_str());
    etd->show();
    etd->setFocus();
    // Connect the closeLogs function of lfd to the delete button of the task
    connect(deleteButton, SIGNAL(clicked()), etd, SLOT(close()));
    connect(etd, SIGNAL(closed()), this, SLOT(editClosed()));

    // Finally connect the task edited signal of the display to the receiving end of the taskwidget
    connect(etd, SIGNAL(sendTaskEdit(QString, URLAndMethod, QString, QString, QString, QDateTime, QString, QString, unsigned int)),
            this, SLOT(acceptTaskEdit(QString, URLAndMethod, QString, QString, QString, QDateTime, QString, QString, unsigned int)));

    // Notify the main window that a log window is open
    *editWindowOpen = true;
}

// Receives a packet of information and rebuilds the task and the task widget with edited info
void VIDTaskWidget::acceptTaskEdit(QString p_title, URLAndMethod p_website, QString p_variantID, QString p_variantName,
                                   QString p_variantSize, QDateTime p_start, QString p_profile, QString p_proxy,
                                   unsigned int frequency) {

    // Rename the old logs file to the new one
    std::rename(std::string(file_paths::TASK_LOG).append("task_logs_").append(task->swh.sourceURL.title).append(task->swh.taskID).append(".txt").c_str(),
                std::string(file_paths::TASK_LOG).append("task_logs_").append(p_website.title).append(task->swh.taskID).append(".txt").c_str());

    // First, change the visuals fo the task widget to match the new task
    title->setText(p_title);
    website->setText(p_website.baseURL);
    variantId->setText(p_variantID);
    variantName->setText(p_variantName);
    variantSize->setText(p_variantSize);
    startAt->setDateTime(p_start);
    taskfreq = frequency;

    // Then, delete the current task and rebuild it with the edited task
    task = new VariantIDTask(p_title.toStdString(), p_website, task->swh.taskID, p_variantID.toStdString(), p_start,
            p_profile.toStdString(), p_proxy.toStdString(), frequency);

    // If a log window is open currently, then close it as well because log file has changed location
    if (logWindowOpen) { lfd->close(); }
}

// Called when the edit task window is closed
void VIDTaskWidget::editClosed() {
    *editWindowOpen = false;
}

// Called when the logFile window is closed
void VIDTaskWidget::logsClosed() {
    *logWindowOpen = false;
}

// EDIT TASK DISPLAY CLASS
// Constructor that builds the window for adding a task
EditTaskDisplay::EditTaskDisplay(const QString& p_title, const QString& p_website, const QString& p_collection,
                                 const QString& p_keywords, const QString& p_colorKeywords, const QString& p_size,
                                 const QDateTime& p_start, const QString& p_profile, const QString& p_proxy, QWidget *parent) : QWidget(parent) {

    // Set window properties
    setFixedSize(500, 270);
    setObjectName("newtaskwindow");
    setWindowTitle(std::string("Edit ").append(p_title.toStdString()).c_str());
    setWindowFlags(Qt::FramelessWindowHint);
    setFocusPolicy(Qt::ClickFocus);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_TranslucentBackground);

    // Add the dark title bar
    dtb = new DarkTitleBar(this, true);

    // Set the stylesheet for the window
    QFile File("./shopifybot/Graphics/stylesheet.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    setStyleSheet(StyleSheet);

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
    // Add the row to the layout
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

    // SIZE & PROXY ROW
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
    eta->setDisplayFormat("[MMMM d, yyyy] hh:mm::ss");
    eta->setDateTime(QDateTime::currentDateTime());
    // Add row to the layout
    sizeEtaLayout->addStretch();
    sizeEtaLayout->addWidget(sizeLabel);
    sizeEtaLayout->addWidget(size);
    sizeEtaLayout->addWidget(etaLabel);
    sizeEtaLayout->addWidget(eta);
    sizeEtaLayout->addStretch();
    mainLayout->addLayout(sizeEtaLayout);

    // FREQUENCY & PROXY ROW
    profileLabel = new QLabel("Profile: ", this);
    profileLabel->setObjectName("addtask_mediocre_text");
    profileLabel->setMaximumWidth(50);
    profile = new QComboBox(this);
    profile->setFixedWidth(100);
    buildProfiles();
    proxyLabel = new QLabel("Proxy: ", this);
    proxyLabel->setObjectName("addtask_mediocre_text");
    proxyLabel->setMaximumWidth(45);
    proxy = new QComboBox(this);
    proxy->setFixedWidth(75);
    buildProxies();
    // Add row to the layout
    frequencyLayout->addWidget(profileLabel);
    frequencyLayout->addWidget(profile);
    frequencyLayout->addWidget(proxyLabel);
    frequencyLayout->addWidget(proxy);
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

    // Set the contents of each edit box
    websites->setCurrentText(p_website);
    collection->setText(p_collection);
    keywords->setText(p_keywords);
    colorKeywords->setText(p_colorKeywords);
    size->setText(p_size);
    eta->setDateTime(p_start);
    profile->setCurrentText(p_profile);
    proxy->setCurrentText(p_proxy);
    title->setText(p_title);

    // Set the layout
    bgLayout->addLayout(mainLayout);
    setLayout(externLayout);

    // Connect the submit button to the attemptToSend slot
    connect(submit, SIGNAL(clicked()), this, SLOT(attemptToSend()));
}

// Tries to send the data in the input fields to the main window to build a new task
void EditTaskDisplay::attemptToSend() {

    // Non-required fields: collection, color keywords
    // Check each required input field to make sure a valid task can be built
    if (keywords->text().isEmpty()) { keywords->setFocus(); return; }
    if (title->text().isEmpty()) { title->setFocus(); return; }

    // Send the signal with all the data to the main window if all required fields have text in them
    emit sendTaskEdit(title->text(), supported_sites::WEBSITES.at(websites->currentText().toStdString()), collection->text(),
                  keywords->text(), colorKeywords->text(), size->text(), eta->dateTime(), profile->currentText(),
                  proxy->currentText());

    // Now close the window because the task edit package has been sent
    close();
}

// Custom close event function that just emits a signal signifying it has closed
void EditTaskDisplay::closeEvent(QCloseEvent *event) {
    // Emit the closed signal and then proceed to cleanup
    emit closed();
    QWidget::closeEvent(event);
}

// TODO: Make sure that when a profile is deleted, all tasks using said profile revert to using "Random"
// Builds the profiles combobox
void EditTaskDisplay::buildProfiles() {
    // First add "Random" as an option
    profile->addItem("Random");

    // Open the profiles.txt
    std::ifstream filein(file_paths::PROFILES_TXT);
    std::string tempStr;

    // Cycle through each line and get the profiles' titles
    while (getline(filein, tempStr)) {
        profile->addItem(tempStr.substr(0, tempStr.find(" :-:")).c_str());
    }

    // Close the file in
    filein.close();
}

// Builds the proxies combobox
void EditTaskDisplay::buildProxies() {
    // First add "Random" as an option
    proxy->addItem("Random");

    // Open the proxies.txt
    std::ifstream filein(file_paths::PROXIES_TXT);
    std::string tempStr;

    // Cycle through each line and get the proxies' ips
    while (getline(filein, tempStr)) {
        proxy->addItem(tempStr.substr(0, tempStr.find(" :-: ")).c_str());
    }

    // Close the filein
    filein.close();
}

// VARIANT ID TASK EDIT DISPLAY CLASS
// Constructor that builds the window for editing a task
VIDTaskEditDisplay::VIDTaskEditDisplay(const QString &p_title, const QString &p_website, const QString &p_variantID,
                                       const QString &p_variantName, const QString &p_variantSize,
                                       const QDateTime &p_start, const QString &p_profile, const QString &p_proxy,
                                       QWidget *parent) : QWidget(parent) {

    // Set window properties
    setFixedSize(500, 230);
    setObjectName("newtaskwindow");
    setWindowTitle(std::string(std::string("Edit \"") + p_title.toStdString() + "\"").c_str());
    setWindowFlags(Qt::FramelessWindowHint);
    setFocusPolicy(Qt::ClickFocus);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_TranslucentBackground);

    // Add the dark title bar
    dtb = new DarkTitleBar(this, true);

    // Set the stylesheet for the window
    QFile File("./shopifybot/Graphics/stylesheet.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());
    setStyleSheet(StyleSheet);

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
    // Add the row to the layout
    startAtLayout->addStretch();
    startAtLayout->addWidget(variantSizeLabel);
    startAtLayout->addWidget(variantSize);
    startAtLayout->addSpacing(10);
    startAtLayout->addWidget(startAtLabel);
    startAtLayout->addWidget(startAt);
    startAtLayout->addSpacing(10);
    startAtLayout->addStretch();
    mainLayout->addLayout(startAtLayout);
    mainLayout->addSpacing(10);

    // PROFILE & PROXY ROW
    profileLabel = new QLabel("Profile: ", this);
    profileLabel->setObjectName("addtask_mediocre_text");
    profileLabel->setFixedWidth(50);
    profile = new QComboBox;
    profile->setFixedWidth(100);
    buildProfiles();
    proxyLabel = new QLabel("Proxy: ", this);
    proxyLabel->setObjectName("addtask_mediocre_text");
    proxyLabel->setFixedWidth(45);
    proxy = new QComboBox;
    proxy->setFixedWidth(75);
    buildProxies();
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

    // Set the contents of each edit box
    websites->setCurrentText(p_website);
    variantID->setText(p_variantID);
    variantTitle->setText(p_variantName);
    variantSize->setText(p_variantSize);
    startAt->setDateTime(p_start);
    profile->setCurrentText(p_profile);
    proxy->setCurrentText(p_proxy);
    title->setText(p_title);

    // Set the layout
    bgLayout->addLayout(mainLayout);
    setLayout(externLayout);

    // Connect the submit button to the attemptToSend slot
    connect(submit, SIGNAL(clicked()), this, SLOT(attemptToSend()));
    // Connect the fill button to the fillfromVariant slot
    connect(checkForNameButton, SIGNAL(clicked()), this, SLOT(fillFromVariant()));
}

// Tries to send the data in the input fields to the main window to edit the task
void VIDTaskEditDisplay::attemptToSend() {

    // Non-required fields: collection, color keywords
    // Check each required input field to make sure a valid task can be built
    if (variantID->text().isEmpty()) { variantID->setFocus(); return; }
    if (variantTitle->text().isEmpty()) { variantTitle->setFocus(); return; }
    if (variantSize->text().isEmpty()) { variantSize->setFocus(); return; }
    if (title->text().isEmpty()) { title->setFocus(); return; }

    // Send the signal with all the data to the main window if all required fields have text in them
    emit sendTaskEdit(title->text(), supported_sites::WEBSITES.at(websites->currentText().toStdString()), variantID->text(),
                      variantTitle->text(), variantSize->text(), startAt->dateTime(), profile->currentText(), proxy->currentText());

    // Now close the window because the task edit package has been sent
    close();
}

// Custom close event function that just emits a signal signifying it has closed
void VIDTaskEditDisplay::closeEvent(QCloseEvent *event) {
    // Emit the closed signal and then proceed to cleanup
    emit closed();
    QWidget::closeEvent(event);
}

// Builds the profiles combobox
void VIDTaskEditDisplay::buildProfiles() {
    // First add "Random" as an option
    profile->addItem("Random");

    // Open the profiles.txt
    std::ifstream filein(file_paths::PROFILES_TXT);
    std::string tempStr;

    // Cycle through each line and get the profiles' titles
    while (getline(filein, tempStr)) {
        profile->addItem(tempStr.substr(0, tempStr.find(" :-:")).c_str());
    }

    // Close the file in
    filein.close();
}

// Builds the proxies combobox
void VIDTaskEditDisplay::buildProxies() {
    // First add "Random" as an option
    proxy->addItem("Random");

    // Open the proxies.txt
    std::ifstream filein(file_paths::PROXIES_TXT);
    std::string tempStr;

    // Cycle through each line and get the proxies' ips
    while (getline(filein, tempStr)) {
        proxy->addItem(tempStr.substr(0, tempStr.find(" :-: ")).c_str());
    }

    // Close the filein
    filein.close();
}

// Fills the add task edits based on the given ID
void VIDTaskEditDisplay::fillFromVariant() {
    // Make sure there is a variant ID
    if (variantID->text().isEmpty()) { variantID->setFocus(); return; }

    // Build a temporary shopify website handler for the given URL and a temporary task ID
    ShopifyWebsiteHandler shopwh(supported_sites::WEBSITES.at(websites->currentText().toStdString()), "temp");

    // Get the tuple of the necessary information
    std::tuple<std::string, std::string, std::string> dataPack = shopwh.getNameSizeImage(variantID->text().toStdString());

    // Finally set the current texts for each lineedit to their respective data
    variantTitle->setText(std::get<0>(dataPack).c_str());
    variantSize->setText(std::get<1>(dataPack).c_str());

    // Remove the temporary task html body
    remove(std::string(std::string(file_paths::HTML_BODY) + supported_sites::WEBSITES.at(websites->currentText().toStdString()).title + "temp.txt").c_str());
}