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

    std::cout << message << std::endl;

    // Close the logging file
    fp.close();
}

// Builds the checkout object's instance variables
Checkout::Checkout(const std::string &p_URL, const std::string &p_cartLink, const std::string &p_logFileLocation,
                   const std::string &p_profile, const std::string &p_proxy, const std::string &p_identifier) :
        url(p_URL), cartLink(p_cartLink), logFileLocation(p_logFileLocation), identifier(p_identifier) {

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
    int proxiesLine = 0;
    QJsonObject proxyObj;
    while(getline(filein, str)) {
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
        proxy = std::string("https://") + proxyObj["proxyip"].toString().toStdString() + ":" + proxyObj["proxyport"].toString().toStdString();
        if (proxyObj["proxyusername"].toString() != "" && proxyObj["proxypassword"].toString() != "") {
            proxyunp = proxyObj["proxyusername"].toString().toStdString() + ":" + proxyObj["proxypassword"].toString().toStdString();
        }
    }

    // If reaches here without a hitch, then everything has been initialized
    allInitialized = true;
}

// Runs the checkout, but only if all variables are initialized
void Checkout::run() {
    std::cout << "Running" << std::endl;

    // Check initialized
    if (!allInitialized) { log("Profile, proxy, or credit card uninitialized! Cannot run."); return; }

    // If initialized, then begin with the loading of the headers for the cart link
    CURL* curl = curl_easy_init();
    std::string shopifyCheckoutURL;
    CURLcode res;
    if (curl) {
        // Build a headers item to submit
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3396.99 Safari/537.36");

        // set the custom set of headers
        res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // First clear the cookie file
        remove(QApplication::applicationDirPath().toStdString().append(file_paths::CHECKOUTCOOKIES_TXT).append(identifier).append(".txt").c_str());

        // Netscape format for the cookie file
        char nline[256];
        snprintf(nline, sizeof(nline), "%s\t%s\t%s\t%s\t%lu\t%s\t%s", ".example.com", "TRUE", "/", "FALSE",
                 (unsigned long)time(nullptr) + 31337UL, "PREF", "hello example");
        // Set other cURL operation settings
        // URL to perform GET request on
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
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
        curl_easy_setopt(curl, CURLOPT_PROXY, proxy);
        if (!proxyunp.empty()) { curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, proxyunp); }
        // Set the timeout
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 4);

        // Now open the file for pulling the body through cURL
        FILE *fp = fopen(QApplication::applicationDirPath().toStdString()
                                 .append(file_paths::CHECKOUTBODY_TXT).append(identifier).append(".txt").c_str(), "wb");
//      FILE *fp = fopen("/Users/samkirkiles/Desktop/shopifybot/shopifybot/WebAccess/Contents/Checkout/cookies2.txt", "wb");
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
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return;
        } else {
            // Get the effective url
            curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &shopifyCheckoutURL);
        }

        // Free the headers and clean up curl
        curl_slist_free_all(headers);
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
        filein.close();
        std::cout << authenticitytoken << std::endl;

        // Reinit the curl session
        curl = curl_easy_init();

        // Set other cURL operation settings
        // URL to perform GET request on
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        // Download the body of the linked URL
        curl_easy_setopt(curl, CURLOPT_NOBODY, 0L);
        // Cookie file to retrieve cookies from
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, QApplication::applicationDirPath().toStdString()
                .append(file_paths::CHECKOUTCOOKIES_TXT).append(identifier).append(".txt").c_str());
        curl_easy_setopt(curl, CURLOPT_COOKIELIST, nline);
        curl_easy_setopt(curl, CURLOPT_COOKIESESSION, true);
        // Allow for redirects
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        // Set the cURL proxy
        curl_easy_setopt(curl, CURLOPT_PROXY, proxy);
        if (!proxyunp.empty()) { curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, proxyunp); }
        // Set the timeout
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 4);

        // Build the data packet to send
        const std::string postfields = std::string("{'utf8':'✓',") +
                                        "'_method':'patch'," +
                                        "'authenticity_token':'" + authenticitytoken + "'," +
                                        "'previous_step':'contact_information',"
                                        "'step':'shipping_method',"
                                        "'checkout[email]':'" + profile["email"].toString().toStdString() + "'," +
                                        "'checkout[buyer_accepts_marketing]':'0',"
                                        "'checkout[shipping_address][first_name]':'" + profile["firstname"].toString().toStdString() + "'," +
                                        "'checkout[shipping_address][last_name]':'" + profile["lastname"].toString().toStdString() + "'," +
                                        "'checkout[shipping_address][address1]':'" + profile["address1"].toString().toStdString() + "'," +
                                        "'checkout[shipping_address][address2]':'" + profile["address2"].toString().toStdString() + "'," +
                                        "'checkout[shipping_address][city]':'" + profile["city"].toString().toStdString() + "'," +
                                        "'checkout[shipping_address][country]':'" + profile["country"].toString().toStdString() + "'," +
                                        "'checkout[shipping_address][province]':'" + profile["province"].toString().toStdString() + "'," +
                                        "'checkout[shipping_address][zip]':'" + profile["zipcode"].toString().toStdString() + "'," +
                                        "'checkout[shipping_address][phone]':'" + profile["phone"].toString().toStdString() + "'," +
                                        "'button':'',"
                                        "'checkout[client_details][browser_width]':'520',"
                                        "'checkout[client_details][browser_height]':'704',"
                                        "'checkout[client_details][javascript_enabled]':'1'}";
        // Attach it to the POST request
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfields);

        // Perform the post request
        res = curl_easy_perform(curl);
        // Close the file
        fclose(fp);

        // Make sure the request succeeded
        if (res != CURLE_OK) {
            log("Error posting checkout information! Aborting checkout...");
            emit setStatus("Failed", "#ed4f47");

            // Free the headers and clean up curl
            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return;
        }
    }
}