//
// Created by Evan Kirkiles on 7/14/18.
//

#ifndef SHOPIFY_BOT_PRODUCT_SCRAPER_HPP
#define SHOPIFY_BOT_PRODUCT_SCRAPER_HPP

// cURL include
#include <curl/curl.h>

// Other includes
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <ctime>
#include <algorithm>
#include <iterator>
#include <boost/algorithm/string.hpp>

#include "constants.hpp"

// Struct that contains a model's colors and size ID's
struct Product {

    std::string title;
    std::string color;

    // Usage: sizes[size] returns the ID for the model of that product
    std::unordered_map<std::string, std::string> sizes;

    // Function to return the id for a given size of the product
    std::string getID(std::string size) {
        size.append(" ");
        for (auto i = sizes.begin(); i != sizes.end(); ++i) {
            std::string format = i.operator*().first;
            format.append(" ");
            if (format.find(size) != format.npos) {
                return i.operator*().second;
            } else {
                std::replace(format.begin(), format.end(), '.', '-');
                if (format.find(size) != format.npos){
                    return i.operator*().second;
                }
            }
        }
        // Shouldn't get here if the size exists
        throw std::runtime_error("Size unavailable.");
    }
};

// Class to interface with the website given
class ShopifyWebsiteHandler {
public:

    // Constructor that initializes the handler with the given URL
    explicit ShopifyWebsiteHandler(const URLAndMethod& url);

    // Function that at the moment prints out the different models it scrapes from the website at given collection
    void getAllModels(const std::string& collection, const std::string& bonusparams="?limit=25");

    // Function that returns the product id from the product's page (the one that has a BUY NOW button)
    // To specify color, they are indexed from the top: highest is 0, then 1, then 2, etc.
    std::string getVariantIDFrom(const std::string& addToURL, const std::string& size, std::string color = "");

    // Function that returns the first product it finds that contains any of the given keywords (title and color).
    // NOTE: Keywords are not case sensitive.
    Product lookForKeywords(const std::string& collection, const std::vector<std::string>& keywords,
                            const std::vector<std::string>& colorKeywords = {""});

private:
    // Instance source variable
    const URLAndMethod sourceURL;

    // Perform a cURL download to get the body of a page
    void performCURL(const std::string& URL);
};


#endif //SHOPIFY_BOT_PRODUCT_SCRAPER_HPP
