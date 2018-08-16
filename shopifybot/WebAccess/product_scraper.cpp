//
// Created by Evan Kirkiles on 7/14/18.
//

#include "product_scraper.hpp"

// Basic constructor that initializes the base website
ShopifyWebsiteHandler::ShopifyWebsiteHandler(const URLAndMethod& url, const std::string& p_taskID) :
        sourceURL(url), taskID(p_taskID) { }

// Function that pulls the HTML source and then searches it for " title: ", printing the lines it finds
void ShopifyWebsiteHandler::getAllModels(const std::string& collection, const std::string& bonusparams) {

    // Begin clock to check how much time each process takes
    std::clock_t start;
    double duration;
    start = std::clock();

    // Also open a new file to write to which logs times of each process
    std::ofstream timeFile;
    timeFile.open(file_paths::TIME_LOG, std::ios::trunc);

    // Run cURL on the website to download the body to a file
    if (sourceURL.method > 100 && sourceURL.method < 200) {
        performCURL(std::string(sourceURL.baseURL).append(collection).append("/products.json").append(bonusparams));
    } else {
        performCURL(std::string(sourceURL.baseURL).append(collection).append(bonusparams));
    }

    // Tell how much time the connection to the website took
    duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    timeFile << duration << " seconds to connect to website. \n";

    // Now parse through the downloaded html file to get the names of all the current items
    std::ifstream searchFile(std::string(file_paths::HTML_BODY) + sourceURL.title + taskID + ".txt");
    std::string str;

    // Also open a new file to write to which will act as the system output
    std::ofstream logFile;
    logFile.open(std::string(file_paths::PRODUCTS_LOG) + sourceURL.title + taskID + ".txt", std::ios::trunc);

    // There are different parse requirements for each different method, hence the if statements
    if (sourceURL.method > 100 && sourceURL.method < 200) {
        std::string sscolor;

        while (getline(searchFile, str)) {

            // If the title identifier is found, print out the line
            unsigned long stringpos = str.find("\"id\":");
            while (stringpos != std::string::npos) {

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

                // In the case of Hanon, the color is located before the "Colourway" string
                if (sourceURL.method == 106) {
                    const unsigned long colourWayPos = str.find(" Colourway");
                    if (colourWayPos != std::string::npos) {
                        const unsigned long asciiLoc = str.substr(0, colourWayPos).rfind("u003e") + 5;
                        sscolor = str.substr(asciiLoc, colourWayPos - asciiLoc);
                        boost::to_upper(sscolor);
                    }
                }

                // In the case of Beatnic, the color is located after the "Color: " string
                if (sourceURL.method == 119) {
                    const unsigned long colourWayPos = str.find("Color: ") + 7;
                    if (colourWayPos != std::string::npos) {
                        const unsigned long asciiLoc = str.find_first_of("u003c", colourWayPos);
                        sscolor = str.substr(colourWayPos, asciiLoc - colourWayPos - 1);
                        boost::to_upper(sscolor);
                    }
                }

                // Finally, check availability
                if (str.find("\"variants\":") > str.find("\"available\":")) {
                    str.erase(0, str.find("\"available\":") + 12);
                    std::string availability = str.substr(0, str.find(','));
                    if (availability == "false") { continue; }
                    // Color comes after a space in the size title
                    if (sourceURL.method == 102) {
                        if (sscolor.empty()) {
                            sscolor = size.substr(size.find(' ') + 1);
                            boost::to_upper(sscolor);
                            logFile << sscolor << "\n" << size.substr(0, size.find(' ')) << " :-: " << id << "\n";
                        } else {
                            logFile << size.substr(0, size.find(' ')) << " :-: " << id << "\n";
                        }
                    // Color is in the title, nothing special to do but reformat the size string
                    } else if (sourceURL.method == 104 || sourceURL.method == 111) {
                        logFile << size.substr(0, size.find(" \\/")) << " :-: " << id << "\n";
                        // Color is located first in the size titles before a ' \/ '
                    } else if (sourceURL.method == 105 || sourceURL.method == 116) {
                        if (sscolor.empty()) {
                            sscolor = size.substr(0, size.find(" \\/"));
                            boost::to_upper(sscolor);
                            logFile << sscolor << "\n" << size.substr(size.find(" \\/") + 4) << " :-: " << id << "\n";
                        } else {
                            logFile << size.substr(size.find(" \\/") + 4) << " :-: " << id << "\n";
                        }
                        // Color is located in the size titles after a ' \/ '
                    } else if (sourceURL.method == 109) {
                        if (sscolor.empty()) {
                            sscolor = size.substr(size.find(" \\/") + 4);
                            boost::to_upper(sscolor);
                            logFile << sscolor << "\n" << size.substr(0, size.find(" \\/")) << " :-: " << id
                                    << "\n";
                        } else {
                            logFile << size.substr(0, size.find(" \\/")) << " :-: " << id << "\n";
                        }
                    // For A Ma Maniere, the color simply needs a little reformatting
                    } else if (sourceURL.method == 112) {
                        const unsigned long slashesPos = str.find("\\/");
                        if (slashesPos != std::string::npos) { size = size.substr(0, slashesPos); }
                        logFile << size << " :-: " << id << "\n";
                    // For everything else, simply write the size and the id
                    } else {
                        // For Billionaire Boys' Club, the processing is done in getting the ID
                        logFile << boost::to_upper_copy(size) << " :-: " << id << "\n";
                    }
                } else {
                    boost::to_upper(size);

                    // In case of color coming in brackets after title
                    if ((sourceURL.method == 101 || sourceURL.method == 112 || sourceURL.method == 113) &&
                            size.find('[') != std::string::npos) {
                        std::string color = size.substr(size.find('[') + 1);
                        color.pop_back();
                        logFile << "\nTITLE: " << size.substr(0, size.find('[') - 1) << ", COLOR: " << color << "\n";
                    } else if (sourceURL.method == 102 || sourceURL.method == 105 || sourceURL.method == 109 ||
                               sourceURL.method == 116) {
                        logFile << "\nTITLE: " << size << ", COLOR: ";
                        sscolor = "";
                    } else if (sourceURL.method == 103 || sourceURL.method == 108) {
                        logFile << "\nTITLE: " << size.substr(0, size.find(" - ")) << ", COLOR: "
                                << size.substr(size.find(" - ") + 3) << "\n";
                    } else if (sourceURL.method == 106 || sourceURL.method == 119) {
                        logFile << "\nTITLE: " << size << ", COLOR: " << sscolor << "\n";
                    } else if (sourceURL.method == 110) {
                        logFile << "\nTITLE: " << size.substr(0, size.find(" : ")) << ", COLOR: " << size.substr(size.find(" : " + 3)) << "\n";
                    } else if (sourceURL.method == 111) {
                        // Color is located in the title after an " IN " substring
                        const unsigned long inPosition = size.find(" IN ");
                        logFile << "\nTITLE: " << size.substr(0, inPosition);
                        size = size.substr(inPosition + 4);
                        logFile << ", COLOR: " << size << "\n";
                    } else if (sourceURL.method == 120 && size.find('(') != std::string::npos) {
                        // Color comes in the title between parentheses '(' color ')'
                        std::string color = size.substr(size.find('(') + 1);
                        color.pop_back();
                        logFile << "\nTITLE: " << size.substr(0, size.find('(') - 1) << ", COLOR: " << color << "\n";
                    } else {
                        // Bodega (104), Shoegallery (107), Addict Miami (114), Anti Social Social Club (115), BBC (117)
                        logFile << "\nTITLE: " << size << "\n";
                    }
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
            if (stringpos != std::string::npos) {

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
                unsigned long newspos = str.find("js-product-card-quickadd-size");
                if (newspos != std::string::npos) {
                    divsFound = 0;

                    // The id is 43 characters over
                    str.erase(0, newspos + 43);
                    std::string id = str.substr(0, str.find('"'));

                    // The size of the given id is two lines down
                    for (int i = 0; i < 2; ++i) { getline(searchFile, str); }
                    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
                    logFile << str << " :-: " << id << "\n";

                    // Variants have now been found
                    varsFound = true;
                } else if (varsFound) {
                    if (str.find("</div>") != std::string::npos) {
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
            if (stringpos != std::string::npos) {

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
                    if (tempPos != std::string::npos) {

                        // Color will be after the tooltip"> substring
                        str.erase(0, tempPos + 9);
                        std::string tempString = boost::to_upper_copy(str.substr(0, str.find('<')));
                        logFile << ", COLOR: " << tempString << "\n";

                        colorFound = true;
                    }
                } else {

                    // Search for sizes
                    tempPos = str.find("data-option-title");
                    if (tempPos != std::string::npos) {

                        // Size and availability would be after class=" identifier
                        tempPos = str.find("data-variant-id=\"");
                        str.erase(0, tempPos + 17);
                        std::string id = str.substr(0, str.find('"'));

                        tempPos = str.find("class=\"");
                        str.erase(0, tempPos + 7);

                        std::string size = str.substr(0, str.find('"'));
                        unsigned long tempoPos = size.find("available");
                        if (tempoPos != std::string::npos) {
                            logFile << size.substr(0, tempoPos - 1) << " :-: " << id << "\n";
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
    timeFile << duration << " seconds to pull the products. \n";

    // Close the files
    timeFile.close();
    logFile.close();
}

// Gets the ID from a product's purchase page
std::string ShopifyWebsiteHandler::getVariantIDFrom(const std::string &addToURL, const std::string& size, std::string color) {

    // Open time logging file
    std::ofstream timeLogs;
    timeLogs.open(file_paths::TIME_LOG, std::ios::trunc);

    if (color.empty()) { color = "ul"; }

    // Begin clock to check how much time this function takes
    std::clock_t begin;
    begin = clock();

    // Download the page's body
    performCURL(std::string(sourceURL.baseURL).append(addToURL));

    // Parse through the downloaded html file
    std::ifstream searchFile(std::string(file_paths::HTML_BODY) + sourceURL.title + taskID + ".txt");
    std::string str;
    std::string id;

    // Read through the html for the website
    while (getline(searchFile, str)) {

        // Look for the handle":" token, this marks the beginning of the variant id list line
        unsigned long stringpos = str.find("handle\":\"");
        if (stringpos != std::string::npos) {
            str.erase(0, stringpos + 9);

            // Find the id, which comes before the size and color
            unsigned long tokenpos = str.find(":[{\"id\":");
            while (tokenpos != std::string::npos) {

                // Retrieve the id for the variant (cannot tell if it is the correct one yet)
                str.erase(0, tokenpos + 8);
                unsigned long temptoken = str.find(',');
                id = str.substr(0, temptoken);
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

                // Returns the id
                break;
            }

            // Only find one handle":" line; if size and color is not there then they do not exist for product
            break;
        }
    }

    // Mark how much time has passed since function began
    timeLogs << (std::clock() - begin) / (double) CLOCKS_PER_SEC << " seconds to get varID. \n";
    timeLogs.close();

    // Placeholder return
    return id;
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
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3396.99 Safari/537.36");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Open the file to write to
    FILE *fp = fopen(std::string(std::string(file_paths::HTML_BODY) + sourceURL.title + taskID + ".txt").c_str(), "wb");
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
                                               const std::vector<std::string>& colorKeywords, const std::string& numresults) {

    // Begin clock to check how much time each process takes
    std::clock_t start;
    start = std::clock();

    // Placeholder Product to be returned
    Product prdct;

    // First, perform a full model scrape on the collection page specified, saved into Contents/products_log.txt
    getAllModels(collection, "?limit=" + numresults);

    // Open time logging file
    std::ofstream timeLogs;
    timeLogs.open(file_paths::TIME_LOG, std::ios::app);

    // Now open products_log.txt and begin parsing through the lines, searching for the keywords in the title
    // Again, some websites have different color placements than others, so I have to account for that.
    std::ifstream searchFile(std::string(file_paths::PRODUCTS_LOG) + sourceURL.title + taskID + ".txt");
    std::string str;

    // Uses a switch function to specify different color locators for different websites
    switch (sourceURL.method) {

        case 104:
        case 107:
        case 114:
        case 115: {
            // In the case of BlendsUS, format is TITLE: [title + color] \n [size] : [id]
            bool titleMatch = false;
            bool prodFound = false;

            // parse through the file looking for TITLE: identifier
            while (getline(searchFile, str)) {

                // If title identifier is found, check for the keywords
                unsigned long stringpos = str.find("TITLE: ");
                if (stringpos != std::string::npos && !prodFound) {
                    std::string title = str.substr(stringpos + 7);
                    for (const std::string &keywd : keywords) {
                        // If a keyword matches, then proceed to check the color
                        if (title.find(boost::to_upper_copy(keywd)) != std::string::npos) {
                            prdct.title = title;
                            titleMatch = true;
                            break;
                        } else {
                            titleMatch = false;
                        }
                    }
                    if (titleMatch) {
                        for (const std::string &keywd : colorKeywords) {
                            // If a keyword matches the color, then product has been found
                            if (str.find(boost::to_upper_copy(keywd)) != std::string::npos) {
                                // Reformat the color if it is found
                                str.erase(0, str.find(boost::to_upper_copy(keywd)));
                                prdct.color = str;
                                prodFound = true;
                                break;
                            }
                        }
                    }
                } else if (stringpos == std::string::npos && prodFound) {
                    // If the product has been found, then this line will be the size and its id
                    prdct.sizes[str.substr(0, str.find(" :-:"))] = str.substr(str.find(" :-: ") + 5);
                } else if (stringpos != std::string::npos && prodFound) {
                    // Mark how much time has passed since function began
                    timeLogs << (std::clock() - start) / (double) CLOCKS_PER_SEC << " seconds to find product. \n";
                    timeLogs.close();
                    return prdct;
                }
            }
        }
        // Billionaire boys club has TITLE: [title] \n [color] \/ [size] :-: [id]
        case 117:
        case 118: {
            bool prodFound = false;
            std::string color;
            std::string foundColor;

            // Parse through the file looking for TITLE: identifier
            while (getline(searchFile, str)) {
                // If title identifier is found, check for the keywords
                unsigned long stringpos = str.find("TITLE: ");
                if (stringpos != std::string::npos && !prodFound) {
                    std::string title = str.substr(stringpos + 7);
                    for (const std::string &keywd : keywords) {
                        // If a keyword matches, then prepare to check the colors
                        if (title.find(boost::to_upper_copy(keywd)) != std::string::npos) {
                            prdct.title = title;
                            prodFound = true;
                            break;
                        } else {
                            prodFound = false;
                        }
                    }
                } else if (stringpos == std::string::npos && prodFound) {
                    // First separate the color from the size (if the size exists)
                    const unsigned long delimPos = str.find(" \\/");
                    if (delimPos == std::string::npos) {
                        color = str.substr(0, str.find(" :-:"));
                    } else {
                        color = str.substr(0, delimPos);
                        if (!foundColor.empty() && foundColor != color) { return prdct; }
                    }

                    // Then, check the color of the line and see if it matches
                    for (const std::string &keywd : colorKeywords) {
                        // If a keyword matches, then add the product ID to the product
                        if (color.find(boost::to_upper_copy(keywd)) != std::string::npos) {
                            prdct.color = color;
                            if (delimPos == std::string::npos) {
                                // If there is only a color and no size, then just put that into the product and return
                                prdct.sizes[""] = str.substr(str.find(" :-: ") + 5);
                                return prdct;
                            } else {
                                // Otherwise, continue looking for products of the same color
                                foundColor = color;
                                str = str.substr(delimPos + 4);
                                const std::string sizeId = str.substr(0, str.find(" :-:"));
                                prdct.sizes[sizeId] = str.substr(str.find(" :-: ") + 5);
                                break;
                            }
                        }
                    }
                } else if (stringpos != std::string::npos && prodFound) {
                    // Mark how much time has passed since fucntion began
                    timeLogs << (std::clock() - start) / (double) CLOCKS_PER_SEC << " seconds to find product. \n";
                    timeLogs.close();
                    return prdct;
                }
            }


        }
        // Default is for format in TITLE: [title], COLOR: [color] \n [size] :-: [id]
        default: {
            bool titleMatch = false;
            bool prodFound = false;

            // Parse through the file looking for TITLE: identifier
            while (getline(searchFile, str)) {

                // If title identifier is found, check for the keywords
                unsigned long stringpos = str.find("TITLE: ");
                if (stringpos != std::string::npos && !prodFound) {
                    std::string title = str.substr(stringpos + 7, str.find(',') - 7);
                    // Remove all extra whitespace from the title
                    title.erase(std::unique(title.begin(), title.end(),
                                            [](char a, char b) { return a == ' ' && b == ' '; }), title.end());
                    for (const std::string &keywd : keywords) {
                        // If a keyword matches, then proceed to check the color
                        if (title.find(boost::to_upper_copy(keywd)) != std::string::npos) {
                            prdct.title = title;
                            titleMatch = true;
                            break;
                        } else {
                            titleMatch = false;
                        }
                    }
                    if (titleMatch) {
                        str.erase(0, str.find("COLOR: ") + 7);
                        for (const std::string &keywd : colorKeywords) {
                            // If a keyword matches the color, then product has been found
                            if (str.find(boost::to_upper_copy(keywd)) != std::string::npos) {
                                str.erase(std::unique(str.begin(), str.end(),
                                                      [](char a, char b) { return a == ' ' && b == ' '; }), str.end());
                                prdct.color = str;
                                prodFound = true;
                                break;
                            }
                        }
                    }
                } else if (stringpos == std::string::npos && prodFound && !str.empty()) {
                    // If the product has been found, then this line will be the size and its id
                    std::string sizeID = str.substr(str.find(" :-: ") + 5);
                    if (sizeID.front() == ':') { sizeID.erase(sizeID.begin()); }
                    prdct.sizes[str.substr(0, str.find(" :-:"))] = sizeID;
                } else if (stringpos != std::string::npos && prodFound) {
                    // Mark how much time has passed since function began
                    timeLogs << (std::clock() - start) / (double) CLOCKS_PER_SEC << " seconds to find product. \n";
                    timeLogs.close();
                    return prdct;
                }
            }
            timeLogs << (std::clock() - start) / (double) CLOCKS_PER_SEC << " seconds to not find product. \n";
            timeLogs << "Could not find product for specified keywords. \n";
            timeLogs.close();
            throw std::runtime_error("Could not find product.");
        }
    }
}

