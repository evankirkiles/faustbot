//
// Created by Evan Kirkiles on 8/27/18.
//

#include "checkout.hpp"

// Log function for the checkout class
void Checkout::log(const std::string &message) {
    // Open the log file for logging
    std::ofstream fp;
    fp.open(std::string(logFileLocation), std::ios::app);

    // Gets the current time
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) % 1000;
    fp << std::put_time(&tm, "[%Y-%m-%d %T.") << std::setfill('0') << std::setw(3) << ms.count() << "] " << message << "\n";

    // Close the logging file
    fp.close();
}

// Builds the checkout object's instance variables
Checkout::Checkout(const std::string &p_cartLink, const std::string &p_logFileLocation,
                   const std::string &p_profile, const std::string &p_proxy,
                   const std::string &p_identifier) :
        cartLink(p_cartLink), logFileLocation(p_logFileLocation), identifier(p_identifier) {

    std::ifstream filein;
    std::string str;
    // Randomizers
    std::random_device rd;
    std::mt19937 mt(rd());
    int randomProfile = -1;
    int randomCCard = -1;
    int randomProxy = -1;

    // Parses through the profiles text to find the profile's JSON information
    filein.open(QApplication::applicationDirPath().append(file_paths::PROFILES_TXT).toStdString().c_str());
    // If the profile is set to random, first choose a random profile
    if (p_profile == "Random") {
        int lines = 0;
        while(getline(filein, str)) {
            lines++;
        }
        // Once line count is finished, pick a random number within that range
        std::uniform_int_distribution<int> dist(1, lines);
        randomProfile = dist(mt);

        // Make sure to reset the file as well
        filein.clear();
        filein.seekg(0, std::ios::beg);
    }
    // Now continue on to searching profile file
    int profileline = 0;
    while(getline(filein, str)) {
        profileline++;
        // Check for profile matches
        const unsigned long indexPos = str.find(" :-: ");
        if (str.substr(0, indexPos) == p_profile || profileline == randomProfile) {
            // On profile match, init the json object of the profile
            str.erase(0, indexPos + 5);
            QJsonDocument jsonDoc = QJsonDocument::fromJson(QString(str.c_str()).toUtf8());
            profile = jsonDoc.object();
            break;
        }
    }
    // Clear the buffers for the next input
    filein.close();
    str = "";

    // Make sure the profile was found as well
    if (profile.isEmpty()) {
        log(std::string("Could not find profile \"").append(p_profile).append("\"!"));
        allInitialized = false;
        return;
    }

    // Parse through the credit cards text to find the credit card JSON information
    filein.open(QApplication::applicationDirPath().append(file_paths::CCARD_TXT).toStdString().c_str());
    // If the credit card of the profile is set to random, first choose a random credit card
    if (profile["ccard"].toString().toStdString() == "Random") {
        int lines = 0;
        while(getline(filein, str)) {
            lines++;
        }
        // Once line count is finished, pick a random number within that range
        std::uniform_int_distribution<int> dist(1, lines);
        randomCCard = dist(mt);

        // Make sure to reset the file as well
        filein.clear();
        filein.seekg(0, std::ios::beg);
    }
    // Now continue on to searching credit cards file
    int ccardLine = 0;
    while(getline(filein, str)) {
        ccardLine++;
        // Check for credit card matches
        const unsigned long indexPos = str.find(" :-: ");
        if (str.substr(0, indexPos) == profile["ccard"].toString().toStdString() || ccardLine == randomCCard) {
            // On credit card match, init the json object of the credit card
            str.erase(0, indexPos + 5);
            QJsonDocument jsonDoc = QJsonDocument::fromJson(QString(str.c_str()).toUtf8());
            ccard = jsonDoc.object();
            break;
        }
    }
    // Clear the buffers for the next input
    filein.close();
    str = "";

    // Make sure a credit card was found as well
    if (ccard.isEmpty()) {
        log(std::string("Could not find credit card \"").append(profile["ccard"].toString().toStdString()).append("\"!"));
        allInitialized = false;
        return;
    }

    // Finally parse through the proxies text to find the proxies JSON information
    filein.open(QApplication::applicationDirPath().append(file_paths::PROXIES_TXT).toStdString().c_str());
    // If the proxy is set to random, first choose a random proxy
    if (p_proxy == "Random") {
        int lines = 0;
        while(getline(filein, str)) {
            lines++;
        }

        // Once line count is finished, pick a random number within that range
        std::uniform_int_distribution<int> dist(1, lines);
        randomProxy = dist(mt);

        // Make sure to reset the file as well
        filein.clear();
        filein.seekg(0, std::ios::beg);
    }

    // Now continue on to searcing the proxies file
    if (p_proxy != "None") {
        int proxiesLine = 0;
        QJsonObject proxyObj;
        while (getline(filein, str)) {
            proxiesLine++;
            // Check for proxy match
            const unsigned long indexPos = str.find(" :-: ");
            if (str.substr(0, indexPos) == p_proxy || proxiesLine == randomProxy) {
                // On proxy match, init the json object of the proxy
                str.erase(0, indexPos + 5);
                QJsonDocument jsonDoc = QJsonDocument::fromJson(QString(str.c_str()).toUtf8());
                proxyObj = jsonDoc.object();
                break;
            }
        }
        // Close the file for the last time
        filein.close();

        // Make sure that a proxy was found as well
        if (proxyObj.isEmpty()) {
            log(std::string("Could not find proxy \"").append(p_proxy).append("\"!"));
            allInitialized = false;
            return;
        } else {
            // If proxy is found, set the proxy string to be used in the requests
            proxy = std::string("") + proxyObj["proxyip"].toString().toStdString() + ":" +
                    proxyObj["proxyport"].toString().toStdString();
            if (proxyObj["proxyusername"].toString() != "" && proxyObj["proxypassword"].toString() != "") {
                proxyunp = proxyObj["proxyusername"].toString().toStdString() + ":" +
                           proxyObj["proxypassword"].toString().toStdString();
            }
        }
    }

    // If reaches here without a hitch, then everything has been initialized
    allInitialized = true;
}

// Runs the checkout, but only if all variables are initialized
void Checkout::run(const std::string& URL) {

    // Check initialized
    if (!allInitialized) { log("Profile, proxy, or credit card uninitialized! Cannot run."); return; }

    // If initialized, then begin with the loading of the headers for the cart link
    CURL* curl = curl_easy_init();
    std::string shopifyCheckoutURL;
    CURLcode res;
    if (curl) {
        // First clear the cookie file
        remove(QApplication::applicationDirPath().toStdString().append(file_paths::CHECKOUTCOOKIES_TXT).append(identifier).append(".txt").c_str());
        emit setStatus("Getting link...", "#7cd4f4");

        // Netscape format for the cookie file
        char nline[256];
        snprintf(nline, sizeof(nline), "%s\t%s\t%s\t%s\t%lu\t%s\t%s", ".example.com", "TRUE", "/", "FALSE",
                 (unsigned long)time(nullptr) + 31337UL, "PREF", "hello example");
        // Set other cURL operation settings
        // URL to perform GET request on
        curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3396.99 Safari/537.36");
        // Download the body of the linked URL
        curl_easy_setopt(curl, CURLOPT_NOBODY, 0L);
        // Cookie file to write to and write all cookies to it
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, QApplication::applicationDirPath().toStdString()
                .append(file_paths::CHECKOUTCOOKIES_TXT).append(identifier).append(".txt").c_str());
        curl_easy_setopt(curl, CURLOPT_COOKIELIST, nline);
        curl_easy_setopt(curl, CURLOPT_COOKIESESSION, true);
        // Allow for redirects
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        // Set the cURL proxy
        if (!proxy.empty()) { curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str()); }
        if (!proxyunp.empty()) { curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, proxyunp.c_str()); }
        // Set the timeout
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);

        // Now open the file for pulling the body through cURL
        FILE *fp = fopen(QApplication::applicationDirPath().toStdString()
                                 .append(file_paths::CHECKOUTBODY_TXT).append(identifier).append(".txt").c_str(), "wb");
        if (fp) {
            // Write the page body to the file
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            // Perform the file download
            res = curl_easy_perform(curl);
            // Close the file
            fclose(fp);
        }

        // Make sure the request succeeded
        if (res != CURLE_OK) {
            log("Error getting checkout link! Aborting checkout...");
            emit setStatus("Failed", "#ed4f47");

            // Free the headers and clean up curl
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return;
        } else {
            // Get the effective url
            char* scu;
            curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &scu);
            shopifyCheckoutURL = scu;
            log(std::string("Got checkout link: ") + shopifyCheckoutURL);
        }

        // Free the headers and clean up curl
        curl_easy_cleanup(curl);

        // Get the authenticity token from the page data
        std::string authenticitytoken;
        std::ifstream filein(QApplication::applicationDirPath().toStdString()
                                     .append(file_paths::CHECKOUTBODY_TXT).append(identifier).append(".txt").c_str());
        int inputs = 0;
        while (getline(filein, authenticitytoken)) {
            if (authenticitytoken.find("input") != std::string::npos) {
                // If there is an input field in the line, check for auth token (should only be 4, we want the third)
                const unsigned long authtokenPos = authenticitytoken.find(R"(name="authenticity_token" value=")");
                if (authtokenPos != std::string::npos) {
                    inputs++;
                    // On the third authenticity token, save it to the string and then break the while loop
                    if (inputs == 3) {
                        authenticitytoken.erase(0, authtokenPos + 33);
                        authenticitytoken = authenticitytoken.substr(0, authenticitytoken.find('"'));
                        break;
                    }
                }
            }
        }
        log(std::string("Got authenticity token: ").append(authenticitytoken));
        filein.close();

        // Reinit the curl session
        curl = curl_easy_init();
        emit setStatus("Posting contact...", "#7cd4f4");

        log("Posting contact information...");
        // Set other cURL operation settings
        // URL to perform POST request on
        curl_easy_setopt(curl, CURLOPT_URL, shopifyCheckoutURL.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3396.99 Safari/537.36");
        // Download the body of the linked URL
        curl_easy_setopt(curl, CURLOPT_NOBODY, 0L);
        // Cookie file to retrieve cookies from
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, QApplication::applicationDirPath().toStdString()
                .append(file_paths::CHECKOUTCOOKIES_TXT).append(identifier).append(".txt").c_str());
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, QApplication::applicationDirPath().toStdString()
                .append(file_paths::CHECKOUTCOOKIES_TXT).append(identifier).append(".txt").c_str());
        curl_easy_setopt(curl, CURLOPT_COOKIELIST, nline);
        curl_easy_setopt(curl, CURLOPT_COOKIESESSION, true);
        // Allow for redirects
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        // Set the cURL proxy
        if (!proxy.empty()) { curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str()); }
        if (!proxyunp.empty()) { curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, proxyunp.c_str()); }
        // Set the timeout
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);

        std::string postfields =
                std::string("utf8=%E2%9C%93") +
                "&_method=patch" +
                "&authenticity_token=" + curl_easy_escape(curl, authenticitytoken.c_str(), static_cast<int>(authenticitytoken.length())) +
                "&previous_step=contact_information" +
                "&checkout[email]=" + curl_easy_escape(curl, profile["email"].toString().toStdString().c_str(), static_cast<int>(profile["email"].toString().toStdString().length())) +
                "&checkout[buyer_accepts_marketing]=0" +
                "&checkout[shipping_address][first_name]=" +
                "&checkout[shipping_address][last_name]=" +
                "&checkout[shipping_address][address1]=" +
                "&checkout[shipping_address][address2]=" +
                "&checkout[shipping_address][city]=" +
                "&checkout[shipping_address][country]=" +
                "&checkout[shipping_address][province]=" +
                "&checkout[shipping_address][zip]=" +
                "&checkout[shipping_address][phone]=" +
                "&checkout[shipping_address][first_name]=" + curl_easy_escape(curl, profile["firstname"].toString().toStdString().c_str(), static_cast<int>(profile["firstname"].toString().toStdString().length())) +
                "&checkout[shipping_address][last_name]=" + curl_easy_escape(curl, profile["lastname"].toString().toStdString().c_str(), static_cast<int>(profile["lastname"].toString().toStdString().length())) +
                "&checkout[shipping_address][address1]=" + curl_easy_escape(curl, profile["address1"].toString().toStdString().c_str(), static_cast<int>(profile["address1"].toString().toStdString().length())) +
                "&checkout[shipping_address][address2]=" + curl_easy_escape(curl, profile["address2"].toString().toStdString().c_str(), static_cast<int>(profile["address2"].toString().toStdString().length())) +
                "&checkout[shipping_address][city]=" + curl_easy_escape(curl, profile["city"].toString().toStdString().c_str(), static_cast<int>(profile["city"].toString().toStdString().length())) +
                "&checkout[shipping_address][country]=" + curl_easy_escape(curl, profile["country"].toString().toStdString().c_str(), static_cast<int>(profile["country"].toString().toStdString().length())) +
                "&checkout[shipping_address][province]=" + curl_easy_escape(curl, profile["province"].toString().toStdString().c_str(), static_cast<int>(profile["province"].toString().toStdString().length())) +
                "&checkout[shipping_address][zip]=" + curl_easy_escape(curl, profile["zipcode"].toString().toStdString().c_str(), static_cast<int>(profile["zipcode"].toString().toStdString().length())) +
                "&checkout[shipping_address][phone]=" + curl_easy_escape(curl, profile["phone"].toString().toStdString().c_str(), static_cast<int>(profile["phone"].toString().toStdString().length())) +
                "&step=shipping_method";

        // Attach it to the POST request
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfields.c_str());

        // Remove the page body data so can bring in new POST data
        remove(QApplication::applicationDirPath().toStdString()
                       .append(file_paths::CHECKOUTBODY_TXT).append(identifier).append(".txt").c_str());
        // Now open the file for pulling the body through cURL
        fp = fopen(QApplication::applicationDirPath().toStdString()
                                 .append(file_paths::CHECKOUTBODY_TXT).append(identifier).append(".txt").c_str(), "wb");
        if (fp) {
            // Write the POST response to the file
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            // Perform the file download
            res = curl_easy_perform(curl);
            // Close the file
            fclose(fp);
        }
        // Make sure the request succeeded
        if (res != CURLE_OK) {
            long response_code;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            std::cout << response_code << std::endl;
            log("Error posting checkout information! Aborting checkout...");
            emit setStatus("Failed", "#ed4f47");

            // Free the headers and clean up curl
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return;
        } else {
            log("Successfully submitted contact information to Shopify servers!");
            // Clean up curl instance
            curl_easy_cleanup(curl);
        }

        // Get the authenticity token from the page data
        authenticitytoken = "";
        filein.open(QApplication::applicationDirPath().toStdString()
                                     .append(file_paths::CHECKOUTBODY_TXT).append(identifier).append(".txt").c_str());
        inputs = 0;
        std::string temporarystring, shippingmethod;
        bool authtokenFound = false;
        bool shippingMethodfound = false;
        while (getline(filein, temporarystring)) {
            if (!authtokenFound && temporarystring.find("authenticity_token") != std::string::npos) {
                // If there is an input field in the line, check for auth token (should only be 4, we want the third)
                const unsigned long authtokenPos = temporarystring.find(R"(name="authenticity_token" value=")");
                if (authtokenPos != std::string::npos) {
                    inputs++;
                    // On the third authenticity token, save it to the string and then break the while loop
                    if (inputs == 3) {
                        authenticitytoken = temporarystring;
                        authenticitytoken.erase(0, authtokenPos + 33);
                        authenticitytoken = authenticitytoken.substr(0, authenticitytoken.find('"'));
                        if (!shippingMethodfound) { break; } else { authtokenFound = true; }
                    }
                }
            }
            // Get the shipping method
            if (!shippingMethodfound && temporarystring.find("checkout_shipping_rate") != std::string::npos) {
                // If there is a value to the line, retrieve it
                const unsigned long shippingRatePos = temporarystring.find("value");
                if (shippingRatePos != std::string::npos) {
                    shippingmethod = temporarystring;
                    shippingmethod.erase(0, shippingRatePos + 7);
                    shippingmethod = shippingmethod.substr(0, shippingmethod.find('"'));
                    if (!authtokenFound) { break; } else { shippingMethodfound = true; }
                }
            }
        }
        log(std::string("Got authenticity token: ").append(authenticitytoken));
        filein.close();

        // Reinit the curl session
        curl = curl_easy_init();
        emit setStatus("Posting...", "#7cd4f4");

        log("Posting shipping method...");
        // Set other cURL operation settings
        // URL to perform POST request on
        curl_easy_setopt(curl, CURLOPT_URL, shopifyCheckoutURL.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3396.99 Safari/537.36");
        // Download the body of the linked URL
        curl_easy_setopt(curl, CURLOPT_NOBODY, 0L);
        // Cookie file to retrieve cookies from
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, QApplication::applicationDirPath().toStdString()
                .append(file_paths::CHECKOUTCOOKIES_TXT).append(identifier).append(".txt").c_str());
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, QApplication::applicationDirPath().toStdString()
                .append(file_paths::CHECKOUTCOOKIES_TXT).append(identifier).append(".txt").c_str());
        curl_easy_setopt(curl, CURLOPT_COOKIELIST, nline);
        curl_easy_setopt(curl, CURLOPT_COOKIESESSION, true);
        // Allow for redirects
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        // Set the cURL proxy
        if (!proxy.empty()) { curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str()); }
        if (!proxyunp.empty()) { curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, proxyunp.c_str()); }
        // Set the timeout
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);

        postfields =
                std::string("utf8=%E2%9C%93") +
                "&_method=patch" +
                "&authenticity_token=" + curl_easy_escape(curl, authenticitytoken.c_str(), static_cast<int>(authenticitytoken.length())) +
                "&previous_step=shipping_method" +
                "&button=" +
                "&checkout[shipping_rate][id]=" + curl_easy_escape(curl, shippingmethod.c_str(), static_cast<int>(shippingmethod.length())) +
                "&checkout[client_details][browser_width]=2560" +
                "&checkout[client_details][browser_height]=775" +
                "&checkout[client_details][javascript_enabled]=1" +
                "&step=payment_method";

        // Attach it to the POST request
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfields.c_str());

        // Remove the page body data so can bring in new POST data
        remove(QApplication::applicationDirPath().toStdString()
                       .append(file_paths::CHECKOUTBODY_TXT).append(identifier).append(".txt").c_str());
        // Now open the file for pulling the body through cURL
        fp = fopen(QApplication::applicationDirPath().toStdString()
                           .append(file_paths::CHECKOUTBODY_TXT).append(identifier).append(".txt").c_str(), "wb");
        if (fp) {
            // Write the POST response to the file
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            // Perform the file download
            res = curl_easy_perform(curl);
            // Close the file
            fclose(fp);
        }
        // Make sure the request succeeded
        if (res != CURLE_OK) {
            long response_code;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            std::cout << response_code << std::endl;
            log("Error posting shipping method! Aborting checkout...");
            emit setStatus("Failed", "#ed4f47");

            // Free the headers and clean up curl
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return;
        } else {
            log("Successfully submitted shipping method to Shopify servers!");
            // Clean up curl instance
            curl_easy_cleanup(curl);
        }

        // Now post the credit card information
        curl = curl_easy_init();
        log("Posting credit card information...");
        emit setStatus("Posting credit card...", "#7cd4f4");

        struct curl_slist *list = NULL;
        list = curl_slist_append(list, "Content-Type: application/json");

        // Set other cURL operatoin settings
        // URL to perform POST request on
        curl_easy_setopt(curl, CURLOPT_URL, "https://elb.deposit.shopifycs.com/sessions");
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3396.99 Safari/537.36");
         curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        // Download the body of the linked URL
        curl_easy_setopt(curl, CURLOPT_NOBODY, 0L);
        // Cookie file to retrieve cookies from
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, QApplication::applicationDirPath().toStdString()
                .append(file_paths::CHECKOUTCOOKIES_TXT).append(identifier).append(".txt").c_str());
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, QApplication::applicationDirPath().toStdString()
                .append(file_paths::CHECKOUTCOOKIES_TXT).append(identifier).append(".txt").c_str());
        curl_easy_setopt(curl, CURLOPT_COOKIELIST, nline);
        curl_easy_setopt(curl, CURLOPT_COOKIESESSION, true);
        // Allow for redirects
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        // Set the cURL proxy
        if (!proxy.empty()) { curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str()); }
        if (!proxyunp.empty()) { curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, proxyunp.c_str()); }
        // Set the timeout
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);

        std::string ccpostfields =
                std::string("{\"credit_card\":") +
                R"({"number":")" + ccard["ccnum"].toString().toStdString() + "\"," +
                R"("name":")" + ccard["ccname"].toString().toStdString() + "\"," +
                R"("month":)" + ccard["ccmonth"].toString().toStdString() + "," +
                R"("year":)" + ccard["ccyear"].toString().toStdString() + "," +
                R"("verification_value":")" + ccard["ccccv"].toString().toStdString() + "\"}}";

        // Now place the JSON ccpostfields into the post request
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, ccpostfields.c_str());

        // Remove the page body data so can bring in new POST data
        remove(QApplication::applicationDirPath().toStdString()
                       .append(file_paths::CHECKOUTBODY_TXT).append(identifier).append(".txt").c_str());
        // Now open the file for pulling the body through cURL
        fp = fopen(QApplication::applicationDirPath().toStdString()
                           .append(file_paths::CHECKOUTBODY_TXT).append(identifier).append(".txt").c_str(), "wb");
        if (fp) {
            // Write the POST response to the file
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            // Perform the file download
            res = curl_easy_perform(curl);
            // Close the file
            fclose(fp);
        }

        // Make sure the request succeeded and if so get the unique shopify session id
        std::string s;
        if (res != CURLE_OK) {
            long response_code;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            std::cout << response_code << std::endl;
            log("Error posting credit card information! Aborting checkout...");
            emit setStatus("Failed", "#ed4f47");

            // Free the headers and clean up curl
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return;
        } else {
            // Check if a unique Shopify checkout ID was received
            filein.open(QApplication::applicationDirPath().toStdString()
                                .append(file_paths::CHECKOUTBODY_TXT).append(identifier).append(".txt").c_str());
            std::string id;
            getline(filein, id);
            if (id.substr(0, 7) == R"({"id":")") {
                std::cout << id << std::endl;
                filein.close();
                s = id.substr(7);
                s.pop_back();
                s.pop_back();
                std::cout << s << std::endl;
                log("Posted credit card information, proceeding to check success...");
                emit setStatus("Processing...", "#7cd4f4");
            } else {
                log("Could not get unique Shopify checkout ID. Connection error maybe? Aborting checkout...");
                emit setStatus("Failed", "#ed4f47");
                curl_easy_cleanup(curl);
                curl_global_cleanup();
                filein.close();
                return;
            }

            // Clean up curl instance
            curl_easy_cleanup(curl);
        }

        // Make a final check of whether the credit card post succeeded
        // Now post the credit card information
        curl = curl_easy_init();
        log("Processing...");
        emit setStatus("Processing...", "#7cd4f4");

        // Set other cURL operatoin settings
        // URL to perform POST request on
        curl_easy_setopt(curl, CURLOPT_URL, shopifyCheckoutURL.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3396.99 Safari/537.36");
        // Download the body of the linked URL
        curl_easy_setopt(curl, CURLOPT_NOBODY, 0L);
        // Cookie file to retrieve cookies from
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, QApplication::applicationDirPath().toStdString()
                .append(file_paths::CHECKOUTCOOKIES_TXT).append(identifier).append(".txt").c_str());
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, QApplication::applicationDirPath().toStdString()
                .append(file_paths::CHECKOUTCOOKIES_TXT).append(identifier).append(".txt").c_str());
        curl_easy_setopt(curl, CURLOPT_COOKIELIST, nline);
        curl_easy_setopt(curl, CURLOPT_COOKIESESSION, true);
        // Allow for redirects
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        // Set the cURL proxy
        if (!proxy.empty()) { curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str()); }
        if (!proxyunp.empty()) { curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, proxyunp.c_str()); }
        // Set the timeout
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);

        std::string authpostfields =
                std::string("utf8=%E2%9C%93") +
                "&_method=patch" +
                "&authenticity_token=" + curl_easy_escape(curl, authenticitytoken.c_str(), static_cast<int>(authenticitytoken.length())) +
                "&previous_step=payment_method" +
                "&step=" +
                "&s=" + curl_easy_escape(curl, s.c_str(), static_cast<int>(s.length())) +
                "&checkout[payment_gateway]=" + curl_easy_escape(curl, paymentgateway.c_str(), static_cast<int>(paymentgateway.length())) +
                "&checkout[credit_card][vault]=false" +
                "&checkout[different_billing_address]=false" +
                "&checkout[remember_me]=false" +
                "&checkout[remember_me]=0" +
                "&checkout[vault_phone]=" + curl_easy_escape(curl, std::string("+").append(profile["phone"].toString().toStdString()).c_str(), static_cast<int>(profile["phone"].toString().toStdString().length()) + 1) +
                "&checkout[total_price]=10" +
                "&complete=1" +
                "&checkout[client_details][browser_width]=2545" +
                "&checkout[client_details][browser_height]=775" +
                "&checkout[client_details][javascript_enabled]=1";

        // Now place the JSON ccpostfields into the post request
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, authpostfields.c_str());

        // Remove the page body data so can bring in new POST data
        remove(QApplication::applicationDirPath().toStdString()
                       .append(file_paths::CHECKOUTBODY_TXT).append(identifier).append(".txt").c_str());
        // Now open the file for pulling the body through cURL
        fp = fopen(QApplication::applicationDirPath().toStdString()
                           .append(file_paths::CHECKOUTBODY_TXT).append(identifier).append(".txt").c_str(), "wb");
        if (fp) {
            // Write the POST response to the file
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            // Perform the file download
            res = curl_easy_perform(curl);
            // Close the file
            fclose(fp);
        }

        // Make sure the request succeeded
        if (res != CURLE_OK) {
            long response_code;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
            std::cout << response_code << std::endl;
            log("Error posting processing! Aborting checkout...");
            emit setStatus("Failed", "#ed4f47");

            // Free the headers and clean up curl
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return;
        } else {
            log("Processing...");

            // Clean up curl instance
            curl_easy_cleanup(curl);
        }

        // Emit finished if it gets here
        curl_global_cleanup();
        emit setStatus("Finished!", "#8dd888");
    }
}