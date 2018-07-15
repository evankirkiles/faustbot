//
// Created by Evan Kirkiles on 7/14/18.
//

#ifndef SHOPIFY_BOT_PRODUCT_SCRAPER_HPP
#define SHOPIFY_BOT_PRODUCT_SCRAPER_HPP

// cURL include
#include <curl/curl.h>
#include <iostream>
#include <fstream>
#include <string>

#include "../supported_sites.hpp"

// Class to interface with the website given
class ShopifyWebsiteHandler {
public:

    // Constructor that initializes the handler with the given URL
    explicit ShopifyWebsiteHandler(const supported_sites::URLAndMethod& url);

    // Function that at the moment prints out the different models it scrapes from the website
    void findModels();

private:
    // Instance source variable
    const supported_sites::URLAndMethod sourceURL;
};


#endif //SHOPIFY_BOT_PRODUCT_SCRAPER_HPP
