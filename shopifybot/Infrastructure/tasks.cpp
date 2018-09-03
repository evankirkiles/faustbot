//
// Created by Evan Kirkiles on 7/18/18.
//

#include "tasks.hpp"

// Task constructor with a very long initializer list for all the instance variables of the Task
Task::Task(const std::string &p_title, const URLAndMethod &p_url, const std::string& p_identifier,
           const std::string& p_collection, const std::vector<std::string> &p_keywords,
           const std::vector<std::string> &p_colorKeywords, const std::string &p_size, const QDateTime& startDate,
           const std::string& p_profile, const std::string& p_proxy,
           const unsigned int p_frequency, const unsigned int p_resultsToCheck)  :
                                             title(p_title),
                                             swh(p_url, p_identifier),
                                             collection(p_collection),
                                             keywords(p_keywords),
                                             colorKeywords(p_colorKeywords),
                                             size(p_size),
                                             startat(startDate),
                                             profile(p_profile),
                                             proxy(p_proxy),
                                             resultsToCheck(p_resultsToCheck),
                                             frequency(p_frequency),
                                             checkout(p_url.checkoutURL,
                                                      QApplication::applicationDirPath().toStdString()
                                                              .append(file_paths::TASKS_LOG).append(p_url.title)
                                                              .append(p_identifier).append(".txt"),
                                                      p_profile, p_proxy, p_identifier) {}

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
            std::string cartLink = std::string(swh.sourceURL.baseURL) + "/cart/" + prdct.getID(size) + ":1";

            // If it gets here, then product has successfully been found
            emit status("Product found!", "#8dd888");
            log("Product found! Link: " + cartLink);

            // Try to order the product
            if (!shouldcontinue) {

                // Delete the files used
                remove(QApplication::applicationDirPath().append(file_paths::HTML_BODY).toStdString().append(swh.sourceURL.title).append(swh.taskID).append(".txt").c_str());
                remove(QApplication::applicationDirPath().append(file_paths::PRODUCTS_LOG).toStdString().append(swh.sourceURL.title).append(swh.taskID).append(".txt").c_str());
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
            remove(QApplication::applicationDirPath().append(file_paths::HTML_BODY).toStdString().append(swh.sourceURL.title).append(swh.taskID).append(".txt").c_str());
            remove(QApplication::applicationDirPath().append(file_paths::PRODUCTS_LOG).toStdString().append(swh.sourceURL.title).append(swh.taskID).append(".txt").c_str());

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
    remove(QApplication::applicationDirPath().append(file_paths::HTML_BODY).toStdString().append(swh.sourceURL.title).append(swh.taskID).append(".txt").c_str());
    remove(QApplication::applicationDirPath().append(file_paths::PRODUCTS_LOG).toStdString().append(swh.sourceURL.title).append(swh.taskID).append(".txt").c_str());
    emit finished();
}

// Logs task messages to the log text file
void Task::log(const std::string &message) {

    // Open the log file for logging
    std::ofstream fp;
    fp.open(std::string(QApplication::applicationDirPath().append(file_paths::TASKS_LOG).toStdString())
                    .append(swh.sourceURL.title).append(swh.taskID).append(".txt").c_str(), std::ios::app);

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

    // Run the checkout!
    checkout.run(url);
}


// Function which interprets the string form of a vector back into the vector
// Ex. "A,B A,C D W" will produce a vector of size 3 with elements "A", "B A", "C D W"
std::vector<std::string> vectorFromString(const std::string& interpret) {

    // Create a stringstream from the interpreted string
    std::stringstream ss(interpret);
    std::vector<std::string> toReturn;

    // Iterate through the stringstream and get all comma-separated elements
    while(ss.good()) {
        std::string sublimstr;
        getline(ss, sublimstr, ',');
        if (*sublimstr.begin() == ' ') {
            toReturn.push_back(sublimstr.substr(1));
        } else {
            toReturn.push_back(sublimstr);
        };
    }
    // Return the vector built from the comma-separated string
    return toReturn;
}

// Function which interprets a vector int a string form
std::string stringFromVector(const std::vector<std::string>& interpret) {

    // Create a string to return and write into it from the vector
    std::string toReturn;
    for (const std::string& i : interpret) {
        toReturn.append(i).append(", ");
    }

    return toReturn;
}

// MARK: VARIANT ID TASK
// Task which runs given a single variant ID, which is continuously checked whether or not it is available.
// Constructor that simply initializes all the instance variables to be used
VariantIDTask::VariantIDTask(const std::string &p_title, const URLAndMethod &p_url, const std::string &p_identifier,
                             const std::string &p_variantID, const QDateTime &p_startat, const std::string &p_profile,
                             const std::string &p_proxy, unsigned int p_frequency) :
        title(p_title),
        swh(p_url, p_identifier),
        variantID(p_variantID),
        startat(p_startat),
        profile(p_profile),
        proxy(p_proxy),
        frequency(p_frequency),
        checkout(p_url.checkoutURL,
                 QApplication::applicationDirPath().toStdString()
                         .append(file_paths::TASKS_LOG).append(p_url.title)
                         .append(p_identifier).append(".txt"),
                 p_profile, p_proxy, p_identifier) { }

// Run function which begins the while loop to be run in a separate thread for each task
void VariantIDTask::run() {
    log("Beginning \"" + title + "\".");
    emit status("Checking...", "#a4ead5");

    shouldcontinue = true;

    // Search for the product periodically at the frequency specified
    while (shouldcontinue) {
        try  {
            // Check if the variant ID is available to be purchased
            const int availabilityCode = swh.productAvailable(variantID);
            if (availabilityCode == 0) {
                // If it is, then order it
                emit status("Placing order...", "#8dd888");
                log("Placing order...");

                // Perform one last check to make sure not interrupted
                if (!shouldcontinue) {

                    // Delete the files used
                    remove(QApplication::applicationDirPath().append(file_paths::HTML_BODY).toStdString().append(swh.sourceURL.title).append(swh.taskID).append(".txt").c_str());
                    remove(QApplication::applicationDirPath().append(file_paths::PRODUCTS_LOG).toStdString().append(swh.sourceURL.title).append(swh.taskID).append(".txt").c_str());
                    emit status("Interrupted.", "#e26c6c");
                    emit finished();
                    return;
                }

                order(std::string(swh.sourceURL.baseURL) + "/cart/" + variantID + ":1");

                // When finished, order should be completed
                log("Task finished.");

                // Remove the files used
                remove(QApplication::applicationDirPath().append(file_paths::HTML_BODY).toStdString().append(swh.sourceURL.title).append(swh.taskID).append(".txt").c_str());
                remove(QApplication::applicationDirPath().append(file_paths::PRODUCTS_LOG).toStdString().append(swh.sourceURL.title).append(swh.taskID).append(".txt").c_str());

                // Successfully finished if it got here
                emit finished();
                return;
            } else if (availabilityCode == 1) {
                // If product is unavailable, then exit the while loop
                throw std::runtime_error("Product out of stock.");
            } else if (availabilityCode == 2) {
                // Some other error occurred with the variant ID
                throw std::runtime_error("Could not locate product.");
            }
        } catch (std::runtime_error& e) {
            log("Failed because: \"" + std::string(e.what()) + "\", checking again in " + std::to_string(frequency) + " seconds.");
            // Sleep the while loop check for another [frequency] seconds
            std::this_thread::sleep_for(std::chrono::seconds(frequency));
            continue;
        }
    }

    // If while loop gets broken, then process is also finished, so emit finished and return to main thread
    emit status("Interrupted.", "#e26c6c");

    // Delete the files used
    remove(QApplication::applicationDirPath().append(file_paths::HTML_BODY).toStdString().append(swh.sourceURL.title).append(swh.taskID).append(".txt").c_str());
    remove(QApplication::applicationDirPath().append(file_paths::PRODUCTS_LOG).toStdString().append(swh.sourceURL.title).append(swh.taskID).append(".txt").c_str());

    emit finished();
}

// Order function which runs the checkout python file
void VariantIDTask::order(const std::string &url) {

    // Run the checkout
    checkout.run(url);
}

// Log function which logs a message to the logfile
void VariantIDTask::log(const std::string &message) {
    // Open the log file for logging
    std::ofstream fp;
    fp.open(std::string(QApplication::applicationDirPath().append(file_paths::TASKS_LOG).toStdString())
                    .append(swh.sourceURL.title).append(swh.taskID).append(".txt").c_str(), std::ios::app);

    // Gets the current time
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) % 1000;
    fp << std::put_time(&tm, "[%Y-%m-%d %T.") << std::setfill('0') << std::setw(3) << ms.count() << "] " << message << "\n";

    // Close the logging file
    fp.close();
}