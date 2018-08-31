//
// Created by Evan Kirkiles on 8/31/18.
//

#ifndef SHOPIFY_BOT_PROXYCHECKER_HPP
#define SHOPIFY_BOT_PROXYCHECKER_HPP

#ifndef QObject
#include <QObject>
#endif
#ifndef QJsonObject
#include <QJsonObject>
#endif
#ifndef QJsonDocument
#include <QJsonDocument>
#endif
#ifndef QApplication
#include <QApplication>
#endif
#ifndef ifstream
#include <fstream>
#endif

// cURL include
#ifndef curl
#include <curl/curl.h>
#endif

#ifndef cout
#include <iostream>
#endif

// Include the global constants
#include <shopifybot/constants.hpp>

// Class containing the proxy checker
class ProxyChecker : public QObject {
    Q_OBJECT
public:
    // Builds the proxy checker for a given proxy
    explicit ProxyChecker(const std::string& index);
    // Runs the proxy check on the chosen URL
    void run(const std::string& URL);

signals:
    // Tells what the status of the proxy check was
    void returnStatus(int status);

private:
    // Instances of parameters passed in
    QJsonObject proxy;
    const std::string index;

    // Must be true for run
    bool allInitialized = false;
};

#endif //SHOPIFY_BOT_PROXYCHECKER_HPP
