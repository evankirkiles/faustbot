#include <iostream>
#include <WebAccess/product_scraper.hpp>

int main() {

    // Run the python script to log into GMail in hopes of reducing captcha probabilities
    FILE* in = popen("python WebAccess/checkout.py ", "r");

    // Start the website handler and search for a product
    ShopifyWebsiteHandler swh(supported_sites::KITH);
    Product prdct = swh.lookForKeywords("/collections/footwear", {"Asics"}, {"White"});

    // When product is found, write the link to a given size in the communications file
    std::string cartLink = std::string(supported_sites::KITH.baseURL) + "/cart/" + prdct.getID("9") + ":1";
    std::ofstream fp;
    fp.open("./WebAccess/communications.txt", std::ios::trunc);
    fp << cartLink;
    fp.close();

    // Wait for subprocess to terminate
    pclose(in);
    return 0;
}