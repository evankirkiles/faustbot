//
// Created by Evan Kirkiles on 7/22/18.
//

#include "taskwidget.hpp"

// Constructor for building the task
TaskWidget::TaskWidget(const QString& p_title, const QString& p_website, const QString& p_keywords,
                       const QString& p_colorKeywords, const QString& p_size, QWidget *parent) :
        title(new QLabel(p_title)),
        website(new QLabel(p_website)),
        keywords(new QLabel(p_keywords)),
        colorKeywords(new QLabel(p_colorKeywords)),
        size(new QLabel(p_size)),
        QFrame(parent) {

    // First, set the style sheet of the frame
    setObjectName("task");

    // Create the layouts for the QLabels
    // Vertical layouts
    auto firstcol = new QVBoxLayout();
    auto thirdcol = new QVBoxLayout();
    // Main horizontal layout
    auto row = new QHBoxLayout();

    // Add the labels and layouts to the main row
    firstcol->addWidget(title);
    firstcol->addWidget(website);
    row->addLayout(firstcol);
    row->addWidget(keywords);
    thirdcol->addWidget(colorKeywords);
    thirdcol->addWidget(size);
    row->addLayout(thirdcol);

    // Set the qframe's layout to the row
    setLayout(row);
}