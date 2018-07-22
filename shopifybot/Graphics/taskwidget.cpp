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
                               task(p_title, p_website, p_collection, p_identifier, p_keywords, p_colorKeywords,
                               p_size, p_quantity, p_resultsToCheck, p_frequency),
                               title(new QLabel(p_title.c_str(), this)),
                               website(new QLabel(p_website.baseURL, this)),
                               size(new QLabel(p_size.c_str(), this)),
                               quantity(new QLabel(std::to_string(p_quantity).c_str(), this)),
                               QFrame(parent) {

    // Initialize the keywords combo boxes
    std::string placeholderkeywords;
    for (const std::string& kwd : p_keywords) { placeholderkeywords.append(kwd + ", "); }
    keywords = new QTextEdit(placeholderkeywords.c_str(), this);
    keywords->setEnabled(false);
    placeholderkeywords = "";
    for (const std::string& kwd : p_colorKeywords) { placeholderkeywords.append(kwd + ", "); }
    colorKeywords = new QTextEdit(placeholderkeywords.c_str(), this);
    colorKeywords->setEnabled(false);

    // First, set the style sheet of the frame
    setObjectName("task");

    // Stylesheet settings for the different labels
    title->setObjectName("task_important_text");
    website->setObjectName("task_mediocre_text");

    // Create the layouts for the QLabels
    // Vertical layouts
    auto firstcol = new QVBoxLayout();
    auto secondcol = new QVBoxLayout();
    auto thirdcol = new QVBoxLayout();
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
    quantitytitle->setObjectName("task_mediocre_title");
    quantityHor->addWidget(quantitytitle);
    quantityHor->addWidget(quantity);
    auto sizeHor = new QHBoxLayout();
    auto sizetitle = new QLabel("Size: ", this);
    sizetitle->setObjectName("task_mediocre_title");
    sizeHor->addWidget(sizetitle);
    sizeHor->addWidget(size);

    // Add the labels and layouts to the main row
    firstcol->addWidget(title);
    firstcol->addWidget(website);
    row->addLayout(firstcol);
    secondcol->addLayout(keywordHor);
    secondcol->addLayout(colorKeywordHor);
    row->addLayout(secondcol);
    thirdcol->addLayout(sizeHor);
    thirdcol->addLayout(quantityHor);
    row->addLayout(thirdcol);

    // Set the qframe's layout to the row
    setLayout(row);
}