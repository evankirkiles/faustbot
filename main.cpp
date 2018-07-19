#include <iostream>
#include <Infrastructure/tasks.hpp>

int main() {

//    // Run the python script to log into GMail in hopes of reducing captcha probabilities
//    FILE* in = popen("python WebAccess/checkout.py ", "r");
//
//    // Start the website handler and search for a product
//    ShopifyWebsiteHandler swh(supported_sites::KITH);
//    Product prdct = swh.lookForKeywords("/collections/footwear", {"Asics"}, {"White"});
//
//    // When product is found, write the link to a given size in the communications file
//    std::string cartLink = std::string(supported_sites::KITH.baseURL) + "/cart/" + prdct.getID("9") + ":1";
//    std::cout << cartLink;
//    std::ofstream fp;
//    fp.open(file_paths::COMMUNICATIONS_FILE, std::ios::trunc);
//    fp << cartLink;
//    fp.close();
//
//    // Wait for subprocess to terminate
//    pclose(in);
//
    Task task("Kith Task", supported_sites::KITH, "/collections/footwear", {"Converse X"}, {"Flame"}, "9", 3, 3);
    task.run();

    return 0;
}