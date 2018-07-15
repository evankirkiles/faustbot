#include <iostream>
#include <WebAccess/product_scraper.hpp>

int main() {

    ShopifyWebsiteHandler swh(supported_sites::WISHATL);
    // swh.getAllModels("/collections/footwear", "?limit=100");

    swh.getVariantIDFrom("/collections/air-jordan/products/air-jordan-1-mid-554724-605?variant=8134851199029", "10", "GYM RED\\/WHITE");

    std::cout << std::endl;
    return 0;
}