//
// Created by Evan Kirkiles on 7/18/18.
//

#include "interface.hpp"

// Creates task at the specified website for the given keywords, adding it to the task list.
void BotInterface::createTask(const std::string& title, const URLAndMethod &website, const std::string& collection,
                              const std::vector<std::string> &keywords, const std::vector<std::string> &colorKeywords,
                              const std::string& size, unsigned int quantity) {

    // Add the task to the vector at the back, with the identifier the position it starts in the vector
    tasks.emplace_back(Task(title, website, std::to_string(tasks.size()), collection, keywords, colorKeywords, size, quantity));
}

// TODO: Figure out how to make the thread remove itself and signify it has finished from the thread vector.

// Runs a task on a new thread
void BotInterface::runTask(unsigned int which) {
    // Creates a thread running the task at the same vector position
    threads[which] = std::thread(&Task::run, tasks[which]);
}