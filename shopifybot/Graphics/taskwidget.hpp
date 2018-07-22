//
// Created by Evan Kirkiles on 7/22/18.
//

#ifndef SHOPIFY_BOT_TASKWIDGET_HPP
#define SHOPIFY_BOT_TASKWIDGET_HPP

#ifndef QWidget
#include <QWidget>
#endif
#ifndef Task
#include "../Infrastructure/tasks.hpp"
#endif
#ifndef QLabel
#include <QLabel>
#endif
#ifndef QVBoxLayout
#include <QVBoxLayout>
#endif
#ifndef QHBoxLayout
#include <QHBoxLayout>
#endif
#ifndef QTextEdit
#include <QTextEdit>
#endif

// Contains the class that builds a Task widget to be added to the scroll area
class TaskWidget : public QFrame {
Q_OBJECT
public:
    // Constructor
    explicit TaskWidget(const std::string& title, const URLAndMethod& website, const std::string& collection,
                        const std::string& p_identifier, const std::vector<std::string>& keywords,
                        const std::vector<std::string>& colorKeywords, const std::string& size, const unsigned int quantity,
                        unsigned int resultsToCheck=constants::BASE_NUMRESULTS,
                        unsigned int frequency=constants::BASE_FREQ, QWidget *parent = 0);

private:
    // Runs the task
    void run();

    // Actual task instance
    Task task;

    // Labels on the task
    QLabel* title;
    QLabel* website;
    QTextEdit* keywords;
    QTextEdit* colorKeywords;
    QLabel* size;
    QLabel* quantity;
};


#endif //SHOPIFY_BOT_TASKWIDGET_HPP
