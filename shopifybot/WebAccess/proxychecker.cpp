//
// Created by Evan Kirkiles on 8/31/18.
//

#include "proxychecker.hpp"

// Constructor that initializes the QJsonObject for the profile
ProxyChecker::ProxyChecker(const std::string &p_index) : index(p_index) {

    // Open the proxy file
    std::ifstream filein(QApplication::applicationDirPath().append(file_paths::PROXIES_TXT).toStdString().c_str());
    std::string str;

    // Parse through the proxy text to find the proxy's JSON information
    while (getline(filein, str)) {
        if (str.empty()) { continue; }
        // Get the title of the proxy and compare it to the given index
        const unsigned long delimPos = str.find(" :-: ");
        if (str.substr(0, delimPos) != index) { continue; }
        str.erase(0, delimPos + 5);

        // Now load the JSON object into the container
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QString(str.c_str()).toUtf8());
        proxy = jsonDoc.object();
        allInitialized = true;
        break;
    }

    // Close the file
    filein.close();
}

// Runs the proxychecker
void ProxyChecker::run(const std::string &URL) {

    // Make sure it proxy object initialized
    if (!allInitialized) { return; }

    // Perform a cURL request on the given URL
    CURL* curl = curl_easy_init();
    CURLcode res;
    if (curl) {
        // Set the cURL operation settings
        // URL to perform GET request on
        curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3396.99 Safari/537.36");
        // Download the body of the linked URL
        curl_easy_setopt(curl, CURLOPT_NOBODY, 0L);
        // Allow for redirects
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        // Set the cURL proxy
        curl_easy_setopt(curl, CURLOPT_PROXY, std::string(proxy["proxyip"].toString().toStdString() + ":" + proxy["proxyport"].toString().toStdString()));
        if (proxy["proxyusername"].toString() != "" && proxy["proxypassword"].toString() != "") {
            curl_easy_setopt(curl, CURLOPT_PROXYUSERPWD, std::string(proxy["proxyusername"].toString().toStdString() + ":" + proxy["proxypassword"].toString().toStdString()));
        }

        // Now open the file for pulling the body through cURL
        FILE *fp = fopen(QApplication::applicationDirPath().toStdString().append(file_paths::PROXYCHECK)
                .append(index).append(".txt").c_str(), "wb");
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
            emit returnStatus(0);
        } else {
            // Make sure there are actually contents in the file
            std::ifstream filein(QApplication::applicationDirPath().toStdString().append(file_paths::PROXYCHECK)
                                         .append(index).append(".txt").c_str(), std::ifstream::ate | std::ifstream::binary);
            if (filein.tellg() != 0) {
                emit returnStatus(1);
            } else {
                // Otherwise curl request went through but got nothing
                emit returnStatus(2);
            }

            // Close the file
            filein.close();
        }
    }
}