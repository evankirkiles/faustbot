//
// Created by Evan Kirkiles on 7/18/18.
//

#include "tasks.hpp"

// Task constructor with a very long initializer list for all the instance variables of the Task
Task::Task(const std::string &p_title, const URLAndMethod &p_url, const std::vector<std::string> &p_keywords,
           const std::vector<std::string> &p_colorKeywords, const std::string &p_size, const unsigned int p_resultsToCheck,
           const unsigned int p_frequency) : title(p_title),
                                             swh(ShopifyWebsiteHandler(p_url)),
                                             keywords(p_keywords),
                                             colorKeywords(p_keywords),
                                             size(p_size),
                                             resultsToCheck(p_resultsToCheck),
                                             frequency(p_frequency) {}

// Run function which begins the while loop to be run in a separate thread for each task.
//void Task::run() {



//}

// Logs task messages to the log text file
void Task::log(const std::string &message) {

    // Open the log file for logging
    std::ofstream fp;
    fp.open(file_paths::TIME_LOG);

    // Gets the current time
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    fp << std::put_time(&tm, "[%d-%m-%Y %H-%M-%S]: ") << message << "\n";

    // Close the logging file
    fp.close();
}