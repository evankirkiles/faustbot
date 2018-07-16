//
// Created by Evan Kirkiles on 7/14/18.
//

#ifndef SHOPIFY_BOT_SUPPORTED_SITES_HPP
#define SHOPIFY_BOT_SUPPORTED_SITES_HPP

// This header holds the base URLs for all the supported websites so far; This standardizes so
// I can use different methods for websites which have different ways of accessing product info.
struct URLAndMethod {
    const char* baseURL;
    const int method;
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

    extern URLAndMethod SOCIALSTATUS;    // Colors are located in the main product's title, with sizes in the variants' titles
    extern URLAndMethod XHIBITION;       // Colors are located in the variants' titles on the right of size
    extern URLAndMethod UNDEFEATED;      // Colors are located in the main product's title, with sizes in the variants' titles
    extern URLAndMethod BLENDS;          // Colors are located in the main product's title, with sizes in the variants' titles
    extern URLAndMethod WISHATL;         // Colors are located in the variants' titles on the left of size

    // MARK: Specialized Shopify sites

    // For Kith, there are collections pages which function in the same way as the base URL page
    extern URLAndMethod KITH;
    // For Bodega, there are also collections pages but base URL page does not have the same function
    extern URLAndMethod BODEGA;

}


#endif //SHOPIFY_BOT_SUPPORTED_SITES_HPP
