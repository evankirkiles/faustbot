//
// Created by Evan Kirkiles on 7/14/18.
//

#include "product_scraper.hpp"

// Basic constructor that initializes the base website
ShopifyWebsiteHandler::ShopifyWebsiteHandler(const URLAndMethod& url) : sourceURL(url) {
}

// Function that pulls the HTML source and then searches it for " title: ", printing the lines it finds
void ShopifyWebsiteHandler::getAllModels(const std::string& collection, const std::string& bonusparams) {

    // Begin clock to check how much time each process takes
    std::clock_t start;
    double duration;
    start = std::clock();

    // Run cURL on the website to download the body to a file
    if (sourceURL.method > 100 && sourceURL.method < 200) {
        performCURL(std::string(sourceURL.baseURL).append(collection).append("/products.json").append(bonusparams));
    } else {
        performCURL(std::string(sourceURL.baseURL).append(collection).append(bonusparams));
    }

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
    if (sourceURL.method > 100 && sourceURL.method < 200) {
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
                }

                logFile << "\n";
                stringpos = str.find("\"id\":");
            }

        }
    } else if (sourceURL.method > 200 && sourceURL.method < 300) {
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
    } else if (sourceURL.method > 200 && sourceURL.method < 300) {
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
    std::cout << std::endl << (std::clock() - start) / (double) CLOCKS_PER_SEC << " seconds total.";
}

// Gets the ID from a product's purchase page
std::string ShopifyWebsiteHandler::getVariantIDFrom(const std::string &addToURL, const std::string& size, std::string color) {

    if (color == "") { color = "null"; }

    // Begin clock to check how much time this function takes
    std::clock_t begin;
    begin = clock();

    // Download the page's body
    performCURL(std::string(sourceURL.baseURL).append(addToURL));

    // Parse through the downloaded html file
    std::ifstream searchFile("./WebAccess/Contents/html_body.txt");
    std::string str;

    // Read through the html for the website
    while (getline(searchFile, str)) {

        // Look for the handle":" token, this marks the beginning of the variant id list line
        unsigned long stringpos = str.find("handle\":\"");
        if (stringpos != str.npos) {
            str.erase(0, stringpos + 9);

            // Find the id, which comes before the size and color
            unsigned long tokenpos = str.find(":[{\"id\":");
            while (tokenpos != str.npos) {

                // Retrieve the id for the variant (cannot tell if it is the correct one yet)
                str.erase(0, tokenpos + 8);
                unsigned long temptoken = str.find(',');
                std::string id = str.substr(0, temptoken);
                str.erase(0, temptoken);

                // Now check the options of the variant to see if it matches the requested color and size
                // Option 1:
                temptoken = str.find(R"("option1":")");
                str.erase(0, temptoken + 11);
                std::string option1 = str.substr(0, str.find(','));
                // Check to make sure option1 is not null and it matches one of the parameters
                if (option1.back() == 'l') { tokenpos = str.find("},{\"id\":"); continue; } else { option1.pop_back(); }
                if (option1 != size && (option1 != color && color != "null")) { tokenpos = str.find("},{\"id\":"); continue; }


                temptoken = str.find(R"("option2":")");
                str.erase(0, temptoken + 11);
                std::string option2 = str.substr(0, str.find(','));
                // Check to make sure option2 is not null and it matches one of the parameters
                if (option2.back() == 'l' && color != "null") { tokenpos = str.find("},{\"id\":"); continue; } else { option2.pop_back(); }
                if (option2 != size && option2 != color) {
                    tokenpos = str.find("},{\"id\":");
                    continue;
                }

                std::cout << std::endl << id << std::endl;
                break;
            }

            // Only find one handle":" line; if size and color is not there then they do not exist for product
            break;
        }
    }

    // Mark how much time has passed since function began
    std::cout << std::endl << (std::clock() - begin) / (double) CLOCKS_PER_SEC << " seconds to get varID.";

    // Placeholder return
    return "egh";
}

// Performs the cURL request and sends the body to html_body.txt
void ShopifyWebsiteHandler::performCURL(const std::string& URL) {

    // Create cURL session
    CURL *curl = curl_easy_init();

    // Set cURL run settings
    // Set the location website
    curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
    // Download the body of the linked URL
    curl_easy_setopt(curl, CURLOPT_NOBODY, 0L);
    // Allow for HTTP 3xx redirects
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

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
}

// Functions similarly to the above getAllModels function except this filters the results first by date
// and then by the prevalence of the keywords in the title.
//std::vector<Product> ShopifyWebsiteHandler::searchFor(const std::string& collection, const std::string& keyword,
//                                                      unsigned int numResults) {
//
//
//
//}

