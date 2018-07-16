#include <iostream>
#include <WebAccess/product_scraper.hpp>

int main() {

    ShopifyWebsiteHandler swh(supported_sites::BODEGA);
    //swh.getAllModels("/collections/footwear", "?limit=100");

    swh.getVariantIDFrom("/collections/footwear/products/clyde-from-the-archive", "9");

    std::cout << std::endl;
    return 0;
}