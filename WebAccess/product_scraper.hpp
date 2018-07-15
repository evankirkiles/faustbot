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

#include "../supported_sites.hpp"

// Struct that contains a model's colors and size ID's
struct Product {

    const std::string title;

    // Usage: models[color][size] returns the ID for the model of that product
    const std::unordered_map<std::string,std::unordered_map<double, std::string>> models;

};

// Class to interface with the website given
class ShopifyWebsiteHandler {
public:

    // Constructor that initializes the handler with the given URL
    explicit ShopifyWebsiteHandler(const URLAndMethod& url);

    // Function that at the moment prints out the different models it scrapes from the website at given collection
    void getAllModels(const std::string& collection, const std::string& bonusparams="?limit=250");

    // Function that returns the product id from the product's page (the one that has a BUY NOW button)
    // To specify color, they are indexed from the top: highest is 0, then 1, then 2, etc.
    std::string getVariantIDFrom(const std::string& addToURL, double size, int whichColor=0);

    // Function that return a multidimensional map of the products it finds for the given keyword
    // Return value usage: returned[0]
    std::vector<Product> searchFor(const std::string& collection, const std::string& keyword, unsigned int numResults);

private:
    // Instance source variable
    const URLAndMethod sourceURL;

    void performCURL(const std::string& URL);
};


#endif //SHOPIFY_BOT_PRODUCT_SCRAPER_HPP
