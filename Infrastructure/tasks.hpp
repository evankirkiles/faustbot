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

// Class containing task-related functions
// Main functionality is a while loop that continuously checks the website for the product
class Task {
public:

    // Constructor that creates an instance of the task for the specified website, title keywords, color keywords,
    // size, results to check, and frequency (default is to check every 30 seconds to preserve performance).
    explicit Task(const std::string& title, const URLAndMethod& url, const std::string& collection,
                  const std::vector<std::string>& keywords, const std::vector<std::string>& colorKeywords,
                  const std::string& size, const unsigned int p_quantity,
                  unsigned int resultsToCheck=constants::BASE_NUMRESULTS, unsigned int frequency=constants::BASE_FREQ);

    // Runs the task, essentially creating a while loop that runs every [frequency] seconds and continuously checks
    // if there exists a product for the given size.
    void run();

private:
    // Title of the task
    const std::string title;

    // Shopify Website Handler
    ShopifyWebsiteHandler swh;
    // Collection in which to search for
    const std::string collection;
    // Keywords to check for in the title of the product
    const std::vector<std::string> keywords;
    // Keywords to check for in the color of the product
    const std::vector<std::string> colorKeywords;
    // Size to check for
    const std::string size;
    // Quantity of products to try for
    const unsigned int quantity;
    // How many results to check on the front of each page (in order of most recent)
    const unsigned int resultsToCheck;
    // How frequently the check should be run (in seconds)
    const unsigned int frequency;

    // Called to log a message to the tasks log file
    void log(const std::string& message);

    // Orders the product from the given URL using PayPal to ensure consistency
    void order(const std::string& url);
};

#endif //SHOPIFY_BOT_TASKS_HPP
