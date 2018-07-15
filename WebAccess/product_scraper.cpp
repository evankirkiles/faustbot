//
// Created by Evan Kirkiles on 7/14/18.
//

#include "product_scraper.hpp"

// Basic constructor that initializes the base website
ShopifyWebsiteHandler::ShopifyWebsiteHandler(const URLAndMethod& url) : sourceURL(url) {}

// Function that pulls the HTML source and then searches it for " title: ", printing the lines it finds
void ShopifyWebsiteHandler::getAllModels(const std::string& collection, const std::string& bonusparams) {

    // Begin clock to check how much time each process takes
    std::clock_t start;
    double duration;
    start = std::clock();

    // Create cURL session
    CURL *curl = curl_easy_init();

    // Set cURL run settings
    // Set the location website
    if (sourceURL.method == 1) {
        curl_easy_setopt(curl, CURLOPT_URL, std::string(sourceURL.baseURL).append(collection).append("/products.json").append(bonusparams).c_str());
    } else {
        curl_easy_setopt(curl, CURLOPT_URL, std::string(sourceURL.baseURL).append(collection).append(bonusparams).c_str());
    }
    // Download the body of the linked URL
    curl_easy_setopt(curl, CURLOPT_NOBODY, 0L);

    // Build the headers for the cURL request
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3396.99 Safari/537.36");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Open the file to write to
    FILE *fp = fopen("./WebAccess/Contents/html_body.txt", "wb");
    if (fp) {

        // Write the page body to the file
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        // Perform file download
        curl_easy_perform(curl);
        // Close the file
        fclose(fp);
    }

    // Free the headers list
    curl_slist_free_all(headers);

    // Clean up cURL
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    // Tell how much time the connection to the website took
    duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    std::cout << std::endl << duration << " seconds to connect to website.";

    // Now parse through the downloaded html file to get the names of all the current items
    std::ifstream searchFile("./WebAccess/Contents/html_body.txt");
    std::string str;

    // Also open a new file to write to which will act as the system output
    std::ofstream logFile;
    logFile.open("./WebAccess/Contents/products_log.txt", std::ios::trunc);

    // There are different parse requirements for each different method, hence the if statements
    if (sourceURL.method == 1) {
        while (getline(searchFile, str)) {

            // If the title identifier is found, print out the line
            unsigned long stringpos = str.find("\"id\":");
            while (stringpos != str.npos) {

                // Print the rest of the string after the found part up until the specified token
                str.erase(0, stringpos + 5);
                unsigned long tokenpos = str.find(",");
                if (str.substr(tokenpos, 10) != ",\"title\":\"") {
                    stringpos = str.find("\"id\":");
                    continue;
                }

                // This substring is the id
                logFile << str.substr(0, tokenpos);

                // Title always comes after id, so title string always comes 9 after
                str.erase(0, tokenpos + 10);

                // This next substring is the title or the size (title always comes before the size)
                logFile << " : " << str.substr(0, str.find("\""));

                // Finally, check availability
                if (str.find("\"variants\":") > str.find("\"available\":")) {
                    str.erase(0, str.find("\"available\":") + 12);
                    std::string availability = str.substr(0, str.find(","));
                    if (availability == "false") {
                        logFile << " : unavailable";
                    }
                } else {

                    // Called whenever a new product is found
                }

                logFile << "\n";
                stringpos = str.find("\"id\":");
            }

        }
    } else if (sourceURL.method == 2) {
        while (getline(searchFile, str)) {

            // If the title identifier is found, print out the line
            unsigned long stringpos = str.find("data-product-tags=\"");
            while (stringpos != str.npos) {

                // Print the rest of the string after the found part up until the specified token
                str.erase(0, stringpos + 19);
                logFile << str.substr(0, str.find("\"")) << "\n";

                stringpos = str.find("data-product-tags=\"");

            }

        }
    } else if (sourceURL.method == 3) {
        while (getline(searchFile, str)) {

            // If the title identifier is found, print out the line
            unsigned long stringpos = str.find("window.BOLD.common.Shopify.saveProduct(\"");
            while (stringpos != str.npos) {

                // Print the rest of the string after the found part up until the specified token
                str.erase(0, stringpos + 40);
                logFile << str.substr(0, str.find("\"")) << "\n";

                stringpos = str.find("window.BOLD.common.Shopify.saveProduct(\"");
            }
        }
    }

    // Tell how much time pulling all the products took
    duration = ((std::clock() - start) / (double) CLOCKS_PER_SEC) - duration;
    std::cout << std::endl << duration << " seconds to pull 100 products.";
    std::cout << std::endl << (std::clock() - start) / (double) CLOCKS_PER_SEC << " seconds total." << std::endl;
}

// Gets the ID from a product's purchase page
std::string ShopifyWebsiteHandler::getIDFrom(const std::string &addToURL) {



}

// Functions similarly to the above getAllModels function except this filters the results first by date
// and then by the prevalence of the keywords in the title.
std::vector<Product> ShopifyWebsiteHandler::searchFor(const std::string& collection, const std::string& keyword,
                                                      unsigned int numResults) {



}

