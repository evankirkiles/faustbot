//
// Created by Evan Kirkiles on 7/18/18.
//

#ifndef SHOPIFY_BOT_TASKS_HPP
#define SHOPIFY_BOT_TASKS_HPP

#ifndef ShopifyWebsiteHandler
#include <WebAccess/product_scraper.hpp>
#endif
#ifndef put_time
#include <iomanip>
#endif
#ifndef thread
#include <thread>
#endif
#ifndef chrono
#include <chrono>
#endif
#ifndef QObject
#include <QObject>
#endif
#ifndef QCoreApplication
#include <QCoreApplication>
#endif
#ifndef QDateTime
#include <QDateTime>
#endif
#ifndef stringstream
#include <sstream>
#endif

// Include the checkout module
#include "WebAccess/checkout.hpp"

// Class containing task-related functions
// Main functionality is a while loop that continuously checks the website for the product
// This is a QObject to allow for QThread functinoality
class Task : public QObject {
        Q_OBJECT
public:

    // TODO: Add a frequency and base num results slider
    // Constructor that creates an instance of the task for the specified website, title keywords, color keywords,
    // size, results to check, and frequency (default is to check every 30 seconds to preserve performance).
    explicit Task(const std::string& title, const URLAndMethod& url, const std::string& identifier,
                  const std::string& collection, const std::vector<std::string>& keywords,
                  const std::vector<std::string>& colorKeywords, const std::string& size, const QDateTime& startat,
                  const std::string& profile, const std::string& proxy,
                  unsigned int frequency=constants::BASE_FREQ, unsigned int resultsToCheck=constants::BASE_NUMRESULTS);

    // Boolean which can be disabled to stop the running of the task
    bool shouldcontinue = false;

    // Changed to public so I can copy all of them over into a new Task easily (no copy constructor bc QObject)
    // Title of the task
    const std::string title;

    // Checkout object
    Checkout checkout;

    // Shopify Website Handler
    ShopifyWebsiteHandler swh;
    // Collection in which to search for
    std::string collection;
    // Keywords to check for in the title of the product
    std::vector<std::string> keywords;
    // Keywords to check for in the color of the product
    std::vector<std::string> colorKeywords;
    // Size to check for
    std::string size;
    // Time at which to start at
    QDateTime startat;
    // Profile identifier to be given to the Python script to use
    std::string profile;
    // Proxy identifier to be given to the Python script to use
    std::string proxy;
    // How many results to check on the front of each page (in order of most recent)
    unsigned int resultsToCheck;
    // How frequently the check should be run (in seconds)
    unsigned int frequency;

public slots:
    // Runs the task, essentially creating a while loop that runs every [frequency] seconds and continuously checks
    // if there exists a product for the given size.
    void run();
signals:
    // Emitted when the task has finished running
    void finished();
    // Emitted to update the status text
    void status(QString text, QString hexColor);

private:
    // Called to log a message to the tasks log file
    void log(const std::string& message);

    // Orders the product from the given URL
    void order(const std::string& url);
};

// Special task which is tailored for knowing variant ID beforehand.
// Repeatedly checks the Shopify cart page, once the product is available then proceeds with the checkout script.
// QOBject to allow for QThread functionality again
class VariantIDTask : public QObject {
    Q_OBJECT
public:

    // Constructor that creates an instance of the task for the specified website, variant ID, etc
    explicit VariantIDTask(const std::string& title, const URLAndMethod& url, const std::string& identifier,
                           const std::string& variantID, const QDateTime& startat, const std::string& profile,
                           const std::string& proxy, unsigned int frequency=constants::BASE_FREQ);

    // Boolean which can be disabled to stop the running of the task
    bool shouldcontinue = false;

    // Checkout object
    Checkout checkout;

    // Title of the task
    const std::string title;

    // Shopify website handler
    ShopifyWebsiteHandler swh;
    // Copy of the variant ID
    std::string variantID;
    // Time at which to start the task
    QDateTime startat;
    // Profile identifier to be given to the Python script to use
    std::string profile;
    // Proxy identifier to be given to the Python script to use
    std::string proxy;
    // How frequently the check should be run (in seconds)
    unsigned int frequency;

public slots:
    // Runs the task, essentially creating a while loop that runs every [frequency] seconds and continuously checks
    // if the variant ID is available to buy through shopify.
    void run();
signals:
    // Emitted when the task has finished running
    void finished();
    // Emitted to update the status text
    void status(QString text, QString hexColor);

private:
    // Called to log a message to the tasks log file
    void log(const std::string& message);
    // Orders the product from the given URL through the python script
    void order(const std::string& url);

};

// Function which interprets the string form of a vector back into the vector
// Ex. "A,B A,C D W" will produce a vector of size 3 with elements "A", "B A", "C D W"
std::vector<std::string> vectorFromString(const std::string& interpret);
std::string stringFromVector(const std::vector<std::string>& interpret);

#endif //SHOPIFY_BOT_TASKS_HPP
