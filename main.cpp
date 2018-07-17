#include <iostream>
#include <WebAccess/product_scraper.hpp>

int main() {

    ShopifyWebsiteHandler swh(supported_sites::BODEGA);
    swh.getAllModels("/collections/footwear", "");

    // swh.getVariantIDFrom("/collections/footwear/products/air-max-95-se", "9");

    std::cout << std::endl;
    return 0;
}