#include <iostream>
#include <WebAccess/product_scraper.hpp>

int main() {

    ShopifyWebsiteHandler swh(supported_sites::KITH);
    // swh.getAllModels("", "");

    // swh.getVariantIDFrom("/collections/footwear/products/air-max-95-se", "9");

    // Run python checkout.py


    Product prdct = swh.lookForKeywords("/collections/footwear", {"ASICS"}, {"WHITE"});
    std::cout << prdct.title << " : " << prdct.color << " : " << prdct.getID("11");

    std::string cartLink = std::string(supported_sites::KITH.baseURL) + "/cart/" + prdct.getID("11") + ":1";
    FILE* in = popen(std::string(std::string("python WebAccess/checkout.py ") + cartLink).c_str(), "r");

    return 0;
}