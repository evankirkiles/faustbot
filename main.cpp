#include <iostream>
#include <WebAccess/product_scraper.hpp>

int main() {

    ShopifyWebsiteHandler swh(supported_sites::BODEGA);
    // swh.getAllModels("", "");

    // swh.getVariantIDFrom("/collections/footwear/products/air-max-95-se", "9");

    // Run python checkout.py
    //FILE* in = popen("python WebAccess/checkout.py https://www.google.com", "r");

    //pclose(in);

    Product prdct = swh.lookForKeywords("/collections/footwear", {"x MITA NMD"}, {""});

    std::cout << prdct.title << " : " << prdct.color << " : " << prdct.getID("10") << std::endl;

    return 0;
}