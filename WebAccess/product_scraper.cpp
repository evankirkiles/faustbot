//
// Created by Evan Kirkiles on 7/14/18.
//

#include "product_scraper.hpp"

// Basic constructor that initializes the base website
ShopifyWebsiteHandler::ShopifyWebsiteHandler(const URLAndMethod& url) : sourceURL(url) { }

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
                unsigned long tokenpos = str.find(',');
                if (str.substr(tokenpos, 10) != R"(,"title":")") {
                    stringpos = str.find("\"id\":");
                    continue;
                }

                // This substring is the id
                std::string id = str.substr(0, tokenpos);

                // Title always comes after id, so title string always comes 9 after
                str.erase(0, tokenpos + 10);

                // This next substring is the title or the size (title always comes before the size)
                std::string size = str.substr(0, str.find(',') - 1);

                // Finally, check availability
                if (str.find("\"variants\":") > str.find("\"available\":")) {
                    str.erase(0, str.find("\"available\":") + 12);
                    std::string availability = str.substr(0, str.find(','));
                    if (availability != "false") {
                        logFile << size << " : " << id << "\n";
                    }
                } else {
                    boost::to_upper(size);
                    logFile << "TITLE: " << size << "\n";
                }
                stringpos = str.find("\"id\":");
            }

        }
    } else if (sourceURL.method > 200 && sourceURL.method < 300) {
        bool prodFound = false;
        bool varsFound = false;
        int divsFound = 0;

        while (getline(searchFile, str)) {

            // While do not have the product found, look for the title identifier
            unsigned long stringpos = str.find("product-card-title-target");
            if (stringpos != str.npos) {

                // The title of the product is the next line
                getline(searchFile, str);
                //str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
                boost::to_upper(str);
                logFile << "TITLE: " << str;

                // The colorway is four lines down
                for (int i = 0; i < 4; ++i) { getline(searchFile, str); }
                //str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
                boost::to_upper(str);
                logFile << ", COLOR: " << str << "\n";

                // Begin looking at variants and their id's
                prodFound = true;
                continue;
            }
            if (prodFound) {
                // Look for the variant identifier
                unsigned long stringpos = str.find("js-product-card-quickadd-size");
                if (stringpos != str.npos) {
                    divsFound = 0;

                    // The id is 43 characters over
                    str.erase(0, stringpos + 43);
                    std::string id = str.substr(0, str.find('"'));

                    // The size of the given id is two lines down
                    for (int i = 0; i < 2; ++i) { getline(searchFile, str); }
                    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
                    logFile << str << " : " << id << "\n";

                    // Variants have now been found
                    varsFound = true;
                } else if (varsFound) {
                    if (str.find("</div>") != str.npos) {
                        ++divsFound;
                        if (divsFound >= 3) {
                            prodFound = false;
                            varsFound = false;
                            divsFound = 0;
                        }
                    }
                }
            }

        }
    } else if (sourceURL.method > 300 && sourceURL.method < 400) {
        bool prodFound = false;
        bool colorFound = false;

        while (getline(searchFile, str)) {

            // While does not have a product found, look for the title identifier
            unsigned long stringpos = str.find("<li id");
            if (stringpos != str.npos) {

                // The title of the product is down 24 lines
                for (int i = 0; i < 24; ++i) { getline(searchFile, str); }
                boost::to_upper(str);
                logFile << "TITLE: " << str;

                prodFound = true;
                colorFound = false;
            }

            if (prodFound) {
                unsigned long tempPos;
                // If the color has not yet been found, search for that before sizes
                if (!colorFound) {

                    tempPos = str.find("tooltip\">");
                    if (tempPos != str.npos) {

                        // Color will be after the tooltip"> substring
                        str.erase(0, tempPos + 9);
                        std::string tempString = boost::to_upper_copy(str.substr(0, str.find('<')));
                        logFile << ", COLOR: " << tempString << "\n";

                        colorFound = true;
                    }
                } else {

                    // Search for sizes
                    tempPos = str.find("data-option-title");
                    if (tempPos != str.npos) {

                        // Size and availability would be after class=" identifier
                        tempPos = str.find("data-variant-id=\"");
                        str.erase(0, tempPos + 17);
                        std::string id = str.substr(0, str.find('"'));

                        tempPos = str.find("class=\"");
                        str.erase(0, tempPos + 7);

                        std::string size = str.substr(0, str.find('"'));
                        unsigned long tempPos = size.find("available");
                        if (tempPos != size.npos) {
                            logFile << size.substr(0, tempPos - 1) << " : " << id << "\n";
                        }
                    }
                }

                // Finally, if the </li> modifier is found with no spaces, the product variants have ended
                if (str == "</li>") {
                    prodFound = false;
                    colorFound = false;
                }
            }
        }
    }

    // Tell how much time pulling all the products took
    duration = ((std::clock() - start) / (double) CLOCKS_PER_SEC) - duration;
    std::cout << std::endl << duration << " seconds to pull the products.";
}

// Gets the ID from a product's purchase page
std::string ShopifyWebsiteHandler::getVariantIDFrom(const std::string &addToURL, const std::string& size, std::string color) {

    if (color.empty()) { color = "ul"; }

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
                if (option1 != size && (option1 != color || color == "ul")) { tokenpos = str.find("},{\"id\":"); continue; }


                temptoken = str.find(R"("option2":)");
                str.erase(0, temptoken + 11);
                std::string option2 = str.substr(0, str.find(','));
                // Check to make sure option2 is not null and it matches one of the parameters
                if (option2.back() == 'l' && color != "ul") { tokenpos = str.find("},{\"id\":"); continue; } else { option2.pop_back(); }
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

// Uses a title keyword and a color keyword to search through all the models on the page and returns the first
// product that matches given keywords. This product will be the most recent one.
Product ShopifyWebsiteHandler::lookForKeywords(const std::string &collection, const std::vector<std::string>& keywords,
                                               const std::vector<std::string>& colorKeywords) {

    // Begin clock to check how much time each process takes
    std::clock_t start;
    start = std::clock();

    // Placeholder Product to be returned
    Product prdct;

    // First, perform a full model scrape on the collection page specified, saved into Contents/products_log.txt
    getAllModels(collection);

    // Now open products_log.txt and begin parsing through the lines, searching for the keywords in the title
    // Again, some websites have different color placements than others, so I have to account for that.
    std::ifstream searchFile("./WebAccess/Contents/products_log.txt");
    std::string str;


    // Uses a switch function to specify different color locators for different websites
    switch (sourceURL.method) {

        // Default is for format in TITLE: [title], COLOR: [color] \n [size] : [id]
        default:
            bool titleMatch = false;
            bool prodFound = false;

            // Parse through the file looking for TITLE: identifier
            while(getline(searchFile, str)) {

                // If title identifier is found, check for the keywords
                unsigned long stringpos = str.find("TITLE: ");
                if (stringpos != str.npos && !prodFound) {
                    std::string title = str.substr(stringpos + 7, str.find(',') - 7);
                    // Remove all extra whitespace from the title
                    title.erase(std::unique(title.begin(), title.end(),
                                            [](char a, char b) {return a == ' ' && b == ' '; }), title.end());
                    for (const std::string& keywd : keywords) {
                        // If a keyword matches, then proceed to check the color
                        if (title.find(boost::to_upper_copy(keywd)) != title.npos) {
                            prdct.title = title;
                            titleMatch = true;
                            break;
                        } else {
                            titleMatch = false;
                        }
                    }
                    if (titleMatch) {
                        str.erase(0, str.find("COLOR: ") + 7);
                        for (const std::string& keywd : colorKeywords) {
                            // If a keyword matches the color, then product has been found
                            if (str.find(boost::to_upper_copy(keywd)) != str.npos) {
                                str.erase(std::unique(str.begin(), str.end(),
                                                        [](char a, char b) {return a == ' ' && b == ' '; }), str.end());
                                prdct.color = str;
                                prodFound = true;
                                break;
                            }
                        }
                    }
                } else if (stringpos == str.npos && prodFound) {
                    // If the product has been found, then this line will be the size and its id
                    prdct.sizes[str.substr(0, str.find(" :"))] = str.substr(str.find(" : ") + 4);
                } else if (stringpos != str.npos && prodFound) {
                    // Mark how much time has passed since function began
                    std::cout << std::endl << (std::clock() - start) / (double) CLOCKS_PER_SEC << " seconds to find product." << std::endl;
                    return prdct;
                }
            }
        std::cout << std::endl << (std::clock() - start) / (double) CLOCKS_PER_SEC << " seconds to not find product." << std::endl;
        std::cout << "Could not find product for specified keywords." << std::endl;
        return prdct;
    }
}

