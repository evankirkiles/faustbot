//
// Created by Evan Kirkiles on 7/14/18.
//

#ifndef SHOPIFY_BOT_SUPPORTED_SITES_HPP
#define SHOPIFY_BOT_SUPPORTED_SITES_HPP

// Constants header as well as base URLs.

// This header holds the base URLs for all the supported websites so far; This standardizes so
// I can use different methods for websites which have different ways of accessing product info.
struct URLAndMethod {
    const char* baseURL;
    const char* checkoutURL;
    const int method;
    const char* title;
};

namespace supported_sites {
// Also create a basic struct which specifies what method to use for getting product info.
// Currently, the integer methods that are specified denote different things:
//      1 : Has a products.json page containing a list of all products in a collection and their id's. (ex. BlendsUS)
//           ^ Will need to specify which collection in the extraURL bit if want to leave front page.
//      2 : Has a "quick add" feature on the base url page that allows directly adding to cart (ex. Kith)
//      3 : Has a grid view with a Shopify saveVariant background (use view source to check) (ex. Bodega)
//           ^ Bodega requires adding an extraURL to specify which type of thing you are looking at (footwear, etc.)

    // MARK: Shopify sites that have a products.json page
    // Unavailable sizes are marked unavailable in normal products.json-website getAllModels functions
    extern const URLAndMethod SOCIALSTATUS;    // Colors are located in the main product's title, with sizes in the variants' titles
    extern const URLAndMethod XHIBITION;       // Colors are located in the variants' titles on the right of size
    extern const URLAndMethod UNDEFEATED;      // Colors are located in the main product's title, with sizes in the variants' titles
    // It should be noted that Undefeated requires being logged into an account to proceed to checkout.
    extern const URLAndMethod BLENDS;          // Colors are located in the main product's title, with sizes in the variants' titles
    extern const URLAndMethod WISHATL;         // Colors are located in the variants' titles on the left of size

    // MARK: Specialized Shopify sites

    // For Kith, there are collections pages which function in the same way as the base URL page
    // Unavailable sizes are simply not shown in the getAllModels function for Kith
    extern const URLAndMethod KITH;
    // For Bodega, there are also collections pages but base URL page does not have the same function
    // Unavailable sizes are marked unavailable in the getAllModels function for Bodega
    extern const URLAndMethod BODEGA;
}

namespace file_paths {
    // Declare file paths here so you can change the location of a file and not have to parse through every
    // usage throughout the program to ensure consistency.
    extern const char* HTML_BODY;

    extern const char* PRODUCTS_LOG;
    extern const char* TIME_LOG;
    extern const char* TASK_LOG;

    // Image locations
    extern const char* LOGO_FILE;
    extern const char* PLAY_IMG;
    extern const char* PLAY2_IMG;
    extern const char* STOP_IMG;
    extern const char* STOP2_IMG;
    extern const char* EDIT_IMG;
    extern const char* EDIT2_IMG;
    extern const char* PLAY_DISBL_IMG;
}

namespace constants {
    // Declare global constants here, mostly 'magic numbers'

    // Standard base frequency used to check the website for product, in seconds.
    extern const unsigned int BASE_FREQ;
    // Standard base number of results to check from the website, by most recent.
    extern const unsigned int BASE_NUMRESULTS;
}




#endif //SHOPIFY_BOT_SUPPORTED_SITES_HPP
