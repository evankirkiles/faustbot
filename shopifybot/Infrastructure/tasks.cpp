//
// Created by Evan Kirkiles on 7/18/18.
//

#include "tasks.hpp"

// Task constructor with a very long initializer list for all the instance variables of the Task
Task::Task(const std::string &p_title, const URLAndMethod &p_url, const std::string& p_identifier,
           const std::string& p_collection, const std::vector<std::string> &p_keywords,
           const std::vector<std::string> &p_colorKeywords, const std::string &p_size, const unsigned int p_quantity,
           const unsigned int p_resultsToCheck, const unsigned int p_frequency) :
                                             title(p_title),
                                             swh(p_url, p_identifier),
                                             collection(p_collection),
                                             keywords(p_keywords),
                                             colorKeywords(p_colorKeywords),
                                             size(p_size),
                                             quantity(p_quantity),
                                             resultsToCheck(p_resultsToCheck),
                                             frequency(p_frequency) {}

// Run function which begins the while loop to be run in a separate thread for each task.
void Task::run() {
    log("Beginning \"" + title + "\".");
    emit status("Searching...", "#a4ead5");

    shouldcontinue = true;

    // Search for the product periodically at the frequency specified
    while (shouldcontinue) {
        try {
            // Try to get the product and the size specified
            Product prdct = swh.lookForKeywords(collection, keywords, colorKeywords, std::to_string(resultsToCheck));
            std::string cartLink = std::string(swh.sourceURL.baseURL) + "/cart/" + prdct.getID(size) + ":" + std::to_string(quantity);

            // If it gets here, then product has successfully been found
            emit status("Product found!", "#8dd888");
            log("Product found! Link: " + cartLink);

            // Try to order the product
            if (!shouldcontinue) {

                // Delete the files used
                remove(std::string("./shopifybot/WebAccess/Contents/html_body_").append(swh.sourceURL.title).append(swh.taskID).append(".txt").c_str());
                remove(std::string("./shopifybot/WebAccess/Contents/products_log_").append(swh.sourceURL.title).append(swh.taskID).append(".txt").c_str());

                emit status("Interrupted.", "#e26c6c");
                emit finished();
                return;
            }

            emit status("Placing order...", "#8dd888");
            log("Placing order...");
            order(cartLink);

            // When finished, order should be completed
            log("Task finished.");

            // Remove the files used
            remove(std::string("./shopifybot/WebAccess/Contents/html_body_").append(swh.sourceURL.title).append(swh.taskID).append(".txt").c_str());
            remove(std::string("./shopifybot/WebAccess/Contents/products_log_").append(swh.sourceURL.title).append(swh.taskID).append(".txt").c_str());

            // Successfully finished if it got here
            emit status("Finished!", "#8dd888");
            emit finished();
            return;

        } catch(std::runtime_error& e) {
            log("Failed because: \"" + std::string(e.what()) + "\", checking again in " + std::to_string(frequency) + " seconds.");
            // Sleep the while loop check for another [frequency] seconds
            std::this_thread::sleep_for(std::chrono::seconds(frequency));
            continue;
        }
    }

    // If while loop gets broken, then process is also finished, so emit finished and return to main thread
    emit status("Interrupted.", "#e26c6c");

    // Delete the files used
    remove(std::string("./shopifybot/WebAccess/Contents/html_body_").append(swh.sourceURL.title).append(swh.taskID).append(".txt").c_str());
    remove(std::string("./shopifybot/WebAccess/Contents/products_log_").append(swh.sourceURL.title).append(swh.taskID).append(".txt").c_str());

    emit finished();
}

// Logs task messages to the log text file
void Task::log(const std::string &message) {

    // Open the log file for logging
    std::ofstream fp;
    fp.open(file_paths::TASK_LOG, std::ios::app);

    // Gets the current time
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) % 1000;
    fp << std::put_time(&tm, "[%Y-%m-%d %T.") << std::setfill('0') << std::setw(3) << ms.count() << "] " << message << "\n";

    // Close the logging file
    fp.close();
}

// Orders the product with paypal from the given URL
void Task::order(const std::string &url) {

    // Essentially just runs the Python Selenium script for the given url
    FILE *fp = popen(std::string(std::string("python3 shopifybot/WebAccess/checkout.py ") + swh.sourceURL.checkoutURL + " " + url).c_str(), "r");

    // Wait for the Python script to finish
    pclose(fp);
}