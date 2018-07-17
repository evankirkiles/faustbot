#include <iostream>
#include <WebAccess/product_scraper.hpp>

int main() {

    ShopifyWebsiteHandler swh(supported_sites::KITH);
    // swh.getAllModels("/collections/footwear", "");

    // swh.getVariantIDFrom("/collections/footwear/products/air-max-95-se", "9");

    // Run python checkout.py
    //FILE* in = popen("python WebAccess/checkout.py https://www.google.com", "r");

    //pclose(in);

    Product prdct = swh.lookForKeywords("/collections/footwear", {"NIKE AIR JORDAN"}, {"Hyper ROYAL"});

    std::cout << std::endl << prdct.title << " : " << prdct.color << std::endl;
//    for (auto i = prdct.sizes.begin(); i != prdct.sizes.end(); ++i) {
//        std::cout << i.operator*().first << " : " << i.operator*().second << std::endl;
//    }
    return 0;
}