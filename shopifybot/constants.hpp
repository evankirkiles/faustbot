//
// Created by Evan Kirkiles on 7/14/18.
//

#ifndef SHOPIFY_BOT_SUPPORTED_SITES_HPP
#define SHOPIFY_BOT_SUPPORTED_SITES_HPP

#ifndef Q_DECLARE_METATYPE
#include <QtCore>
#endif
#ifndef unordered_map
#include <unordered_map>
#endif
#ifndef QStringList
#include <QStringList>
#endif

// Constants header as well as base URLs.

// This header holds the base URLs for all the supported websites so far; This standardizes so
// I can use different methods for websites which have different ways of accessing product info.
struct URLAndMethod {
    const char* baseURL = "";
    const char* checkoutURL = "";
    int method = 0;
    const char* title = "";
};

// Declare the URLAndMethod as a metatype so can send it in a signal
Q_DECLARE_METATYPE(URLAndMethod);

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
    extern const URLAndMethod HANON;           // Colors are located in the products.json before the term "Colourway"; really inconvenient
    // It should be noted that Hanon requires being logged into an account to proceed to checkout.
    // Also, Hanon has both UK and US sizes, so when specifying the size one should precede it with "UK " or "US "
    extern const URLAndMethod SHOEGALLERY;     // Color is located in the main product's title, no way to remove it unfortunately
    extern const URLAndMethod SHOPNICEKICKS;   // Colors are located in the main product's title, after the ' - ' substring
    extern const URLAndMethod NOTRESHOP;       // Colors are located in the product's options, easy to find
    extern const URLAndMethod TWELVEAMRUN;     // Colors are located in the product's title, after a ' : '
    extern const URLAndMethod EIGHTEENMONTROSE;// Colors are located in the product's sizes, after a ' \/ ' but before a ' \/ '
    extern const URLAndMethod AMAMANIERE;      // Colors are located in the product's title, after a '[' and before a ']'
    extern const URLAndMethod APBSTORE;        // Colors are located in the product's title, after a '[' and before a ']'
    extern const URLAndMethod ADDICTMIAMI;     // Colors are located in the product's title, inseparable unfortunately
    extern const URLAndMethod ASSC;            // Colors are located in the product's title, inseparable unfortunately
    extern const URLAndMethod EXCLUSIVEDNA;    // Colors are located in the product's sizes before a ' \/ '
    extern const URLAndMethod BBCICECREAM;     // Colors are located in the product's sizes before a ' \/ '
    extern const URLAndMethod BAPE;            // Colors are located in the product's sizes before a ' \/ '
    // It should be noted that Bape's products.json do not correspond to the items on the page. There may be some overlap
    // but for the most part it is not efficient to scrape this way. Solutions include maybe just scraping the entire
    // products list, which is extremely inefficient but works? I guess. Bape only has 150 products so I can maneuver
    // around their catalogue semiquickly with ?limit= and ?page= ('&' for a combo).
    // TODO: Make Bape search more efficient
    extern const URLAndMethod BEATNIC;          // Colors are located in the products.json after the term "Color: "
    // TODO: Debug Beatnic scraping as well, currently UNSUPPORTED
    extern const URLAndMethod BLACKMARKETUSA;   // Colors are located in the product's title, after a '(' but before a ')'

    // MARK: Specialized Shopify sites

    // For Kith, there are collections pages which function in the same way as the base URL page
    // Unavailable sizes are simply not shown in the getAllModels function for Kith
    extern const URLAndMethod KITH;
    // For Bodega, there are also collections pages but base URL page does not have the same function
    // Unavailable sizes are marked unavailable in the getAllModels function for Bodega
    extern const URLAndMethod BODEGA;


    // Array containing a reference to each of these constants at a given name
    extern const std::unordered_map<std::string, URLAndMethod>WEBSITES;
    extern const std::unordered_map<std::string, std::string>WEBSITES_BWD;

    // StringList which can be set in the combobox for all the supported sites
    extern const QStringList ssStringList;
}

namespace file_paths {
    // Declare file paths here so you can change the location of a file and not have to parse through every
    // usage throughout the program to ensure consistency.
    extern const char* HTML_BODY;

    extern const char* PRODUCTS_LOG;
    extern const char* TIME_LOG;
    extern const char* TASK_LOG;

    extern const char* STYLESHEET;
    extern const char* COLORSTYLESHEET;
    extern const char* COLORSTYLESHEET_WITHVARS;
    extern const char* COLORSTYLESHEET_DEFAULT;

    extern const char* PROFILES_TXT;
    extern const char* TEMPPROFILES_TXT;
    extern const char* CCARD_TXT;
    extern const char* TEMPCCARD_TXT;
    extern const char* PROXIES_TXT;
    extern const char* TEMPPROXIES_TXT;
    extern const char* COOKIES_TXT;

    extern const char* CONTENTS_DIR;
    extern const char* LOGS_DIR;

    // Image locations
    extern const char* LOGO_FILE;
    extern const char* PLAY_IMG;
    extern const char* PLAY2_IMG;
    extern const char* STOP_IMG;
    extern const char* STOP2_IMG;
    extern const char* EDIT_IMG;
    extern const char* EDIT2_IMG;
    extern const char* PLAY_DISBL_IMG;
    extern const char* REPLAY_IMG;
    extern const char* REPLAY2_IMG;
    extern const char* DELETE_IMG;
    extern const char* DELETE2_IMG;
    extern const char* LOGS_IMG;
    extern const char* LOGS2_IMG;
    extern const char* EXIT_IMG;
    extern const char* EXIT2_IMG;
    extern const char* MOREINFO_IMG;
    extern const char* MOREINFO2_IMG;
    extern const char* ADD_IMG;
    extern const char* ADD2_IMG;
    extern const char* MINUS_IMG;
    extern const char* MINUS2_IMG;
    extern const char* MINUS2_IMG;
    extern const char* DUPLICATE_IMG;
    extern const char* DUPLICATE2_IMG;
    extern const char* REFRESH_IMG;
    extern const char* REFRESH2_IMG;
    extern const char* DISABLEDREFRESH_IMG;
    extern const char* PROXYON_IMG;
    extern const char* PROXYOFF_IMG;
    extern const char* PROXYNEUTRAL_IMG;
    extern const char* REFRESHICON_IMG;
    extern const char* REFRESHICON2_IMG;
    extern const char* DEFAULTTHUMB_IMG;
    extern const char* GEAR_IMG;
}

namespace constants {
    // Declare global constants here, mostly 'magic numbers'

    // Standard base frequency used to check the website for product, in seconds.
    extern const unsigned int BASE_FREQ;
    // Standard base number of results to check from the website, by most recent.
    extern const unsigned int BASE_NUMRESULTS;

    // Array of variables settable through the Color Customizer
    extern const std::vector<std::string> CUSTOMIZEVARS;
}




#endif //SHOPIFY_BOT_SUPPORTED_SITES_HPP
