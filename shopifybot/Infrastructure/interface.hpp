//
// Created by Evan Kirkiles on 7/18/18.
//

#ifndef SHOPIFY_BOT_INTERFACE_HPP
#define SHOPIFY_BOT_INTERFACE_HPP

// Include threads
#include <thread>

// Include custom classes
#ifndef ShopifyWebsiteHandler
#include "WebAccess/product_scraper.hpp"
#endif
#ifndef Task
#include "tasks.hpp"
#endif

// This class will contain the interface that takes a shoe and a size and searches a website for it.
class BotInterface {
public:

    // Constructor that sets the URLAndMethod of the interface
    explicit BotInterface() = default;

    // Adds a task that will continuously check for a product, and, if found, adds it to the cart.
    // This task has to be started off the task list in its own thread for efficient processing.
    void createTask(const std::string& title, const URLAndMethod& website, const std::string& collection,
                    const std::vector<std::string>& keywords, const std::vector<std::string>& colorKeywords,
                    const std::string& size, unsigned int quantity);

    // Runs the task at the specified index on the thread and tasks vector
    void runTask(unsigned int which);

private:
    // Vectors that contain threads and their tasks
    std::vector<Task> tasks;
    std::vector<std::thread> threads;
};

#endif //SHOPIFY_BOT_INTERFACE_HPP
