//
// Created by Evan Kirkiles on 7/22/18.
//

#include "taskwidget.hpp"

// Constructor for building the task
TaskWidget::TaskWidget(const std::string& p_title, const URLAndMethod& p_website, const std::string& p_identifier,
                       const std::string& p_collection, const std::vector<std::string>& p_keywords,
                       const std::vector<std::string>& p_colorKeywords, const std::string& p_size,
                       const unsigned int p_quantity, unsigned int p_resultsToCheck,
                       unsigned int p_frequency, QWidget *parent) :
                               task(p_title, p_website, p_identifier, p_collection, p_keywords, p_colorKeywords,
                               p_size, p_quantity, p_resultsToCheck, p_frequency),
                               title(new QLabel(p_title.c_str(), this)),
                               website(new QLabel(p_website.baseURL, this)),
                               collection(new QLabel(p_collection.c_str(), this)),
                               size(new QLabel(p_size.c_str(), this)),
                               quantity(new QLabel(std::to_string(p_quantity).c_str(), this)),
                               QFrame(parent) {

    // Initialize the keywords combo boxes
    std::string placeholderkeywords;
    for (const std::string& kwd : p_keywords) { placeholderkeywords.append(kwd + ", "); }
    keywords = new QTextEdit(placeholderkeywords.c_str(), this);
    keywords->setReadOnly(true);
    keywords->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    keywords->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    placeholderkeywords = "";
    for (const std::string& kwd : p_colorKeywords) { placeholderkeywords.append(kwd + ", "); }
    colorKeywords = new QTextEdit(placeholderkeywords.c_str(), this);
    colorKeywords->setReadOnly(true);
    colorKeywords->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    colorKeywords->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // First, set the style sheet of the frame
    setObjectName("task");

    // Stylesheet settings for the different labels
    title->setObjectName("task_important_text");
    website->setObjectName("task_mediocre_text");
    collection->setObjectName("task_mediocre_text");
    size->setObjectName("task_important_var");
    quantity->setObjectName("task_important_var");

    // Create the layouts for the QLabels
    // Vertical layouts
    auto firstcol = new QVBoxLayout();
    auto secondcol = new QVBoxLayout();
    auto thirdcol = new QVBoxLayout();
    auto fourthcol = new QVBoxLayout();
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

    // Set the qframe's layout to the row
    setLayout(row);

    // Connect clicking play to beginning the task.
    connect(play, SIGNAL(runTask()), this, SLOT(run()));
}

// Tells the event loop to run the task
void TaskWidget::run() {

    // Initializes the thread and moves the task onto it
    taskthread = new QThread;
    Task* temptask = new Task(task.title, task.swh.sourceURL, task.swh.taskID, task.collection, task.keywords,
            task.colorKeywords, task.size, task.quantity, task.resultsToCheck, task.frequency);
    temptask->moveToThread(taskthread);

    // Remove all connections to play so do not set multiple shouldcontinues
    play->disconnect();
    connect(play, SIGNAL(runTask()), this, SLOT(run()));

    // Connects all the necessary signals and slots for communication between the two
    connect(taskthread, SIGNAL(started()), temptask, SLOT(run()));
    connect(play, &ClickableCheckableImage::interrupt, [this, temptask] () {
        std::cout << "cmon bruh" << std::endl;
        play->disable();
        temptask->shouldcontinue = false;});
    connect(temptask, SIGNAL(finished()), play, SLOT(enable()));
    connect(temptask, SIGNAL(finished()), taskthread, SLOT(quit()));
    connect(temptask, SIGNAL(finished()), temptask, SLOT(deleteLater()));
    connect(temptask, SIGNAL(finished()), taskthread, SLOT(deleteLater()));

    // Begin the thread!
    taskthread->start();
}