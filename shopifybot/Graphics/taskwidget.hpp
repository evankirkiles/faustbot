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

// Contains the class that builds a Task widget to be added to the scroll area
class TaskWidget : public QFrame {
Q_OBJECT
public:
    // Constructor
    explicit TaskWidget(const QString& title, const QString& website, const QString& keywords,
                        const QString& colorKeywords, const QString& size, QWidget *parent = 0);

private:
    // Runs the task
    void run();

    // Labels on the task
    QLabel* title;
    QLabel* website;
    QLabel* keywords;
    QLabel* colorKeywords;
    QLabel* size;
};

#endif //SHOPIFY_BOT_TASKWIDGET_HPP
