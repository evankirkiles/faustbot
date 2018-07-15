//
// Created by Evan Kirkiles on 7/14/18.
//

#include "product_scraper.hpp"

// Basic constructor that initializes the base website
ShopifyWebsiteHandler::ShopifyWebsiteHandler(const supported_sites::URLAndMethod& url) : sourceURL(url) {}

// Function that pulls the HTML source and then searches it for " title: ", printing the lines it finds
void ShopifyWebsiteHandler::findModels() {

    // Create cURL session
    CURL *curl = curl_easy_init();

    // Set cURL run settings
    // Set the location website
    if (sourceURL.method == 1) {
        curl_easy_setopt(curl, CURLOPT_URL, std::string(sourceURL.baseURL).append(sourceURL.extraURL).append("/products.json").append(sourceURL.evenMoreURL).c_str());
    } else {
        curl_easy_setopt(curl, CURLOPT_URL, std::string(sourceURL.baseURL).append(sourceURL.extraURL).c_str());
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

    // Now parse through the downloaded html file to get the names of all the current items
    std::ifstream searchFile("./WebAccess/Contents/html_body.txt");
    std::string str;

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
                std::cout << std::endl << str.substr(0, tokenpos);

                // Title always comes after id, so title string always comes 9 after
                str.erase(0, tokenpos + 10);

                // This next substring is the title or the size (title always comes before the size)
                std::cout << " : " << str.substr(0, str.find("\""));

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
                std::cout << std::endl << str.substr(0, str.find("\""));

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
                std::cout << std::endl << str.substr(0, str.find("\""));

                stringpos = str.find("window.BOLD.common.Shopify.saveProduct(\"");
            }

        }
    }
}

