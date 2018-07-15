#include <iostream>
#include <WebAccess/product_scraper.hpp>

int main() {

    ShopifyWebsiteHandler swh(supported_sites::WISHATL);
    swh.getAllModels("/collections/footwear", "?limit=100");

    return 0;
}