//
// Created by Evan Kirkiles on 8/27/18.
//

#include "checkout.hpp"

// Log function for the checkout class
void Checkout::log(const std::string &message) {
    // Open the log file for logging
    std::ofstream fp;
    fp.open(std::string(logFileLocation.c_str()), std::ios::app);

    // Gets the current time
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) % 1000;
    fp << std::put_time(&tm, "[%Y-%m-%d %T.") << std::setfill('0') << std::setw(3) << ms.count() << "] " << message << "\n";

    // Close the logging file
    fp.close();
}

// Builds the checkout object's instance variables
Checkout::Checkout(const std::string &p_URL, const std::string &p_cartLink, const std::string &p_logFileLocation,
                   const std::string &p_profile, const std::string &p_proxy) :
        url(p_URL), cartLink(p_cartLink), logFileLocation(p_logFileLocation) {

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
    while(getline(filein, str)) {
        proxiesLine++;
        // Check for proxy match
        const unsigned long indexPos = str.find(" :-: ");
        if (str.substr(0, indexPos) == p_proxy || proxiesLine == randomProxy) {
            // On proxy match, init the json object of the proxy
            str.erase(0, indexPos + 5);
            QJsonDocument jsonDoc = QJsonDocument::fromJson(QString(str.c_str()).toUtf8());
            proxy = jsonDoc.object();
            break;
        }
    }
    // Close the file for the last time
    filein.close();

    // Make sure that a proxy was found as well
    if (proxy.isEmpty()) {
        log(std::string("Could not find proxy \"").append(p_proxy).append("\"!"));
        allInitialized = false;
        return;
    }

    // If reaches here without a hitch, then everything has been initialized
    allInitialized = true;
}

//