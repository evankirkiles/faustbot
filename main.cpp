#include <iostream>
#include <WebAccess/product_scraper.hpp>

int main() {

    // Run the python script to log into GMail in hopes of reducing captcha probabilities
//     FILE* in = popen("python WebAccess/checkout.py ", "r");

    // Start the website handler and search for a product
    ShopifyWebsiteHandler swh(supported_sites::UNDEFEATED);
    Product prdct = swh.lookForKeywords("/collections/footwear", {"Replicant"}, {"Brown"});

    // When product is found, write the link to a given size in the communications file
    std::string cartLink = std::string(supported_sites::UNDEFEATED.baseURL) + "/cart/" + prdct.getID("9") + ":1";
    std::cout << cartLink;
//    std::ofstream fp;
//    fp.open(file_paths::COMMUNICATIONS_FILE, std::ios::trunc);
//    fp << cartLink;
//    fp.close();

    // Wait for subprocess to terminate
//    pclose(in);
    return 0;
}