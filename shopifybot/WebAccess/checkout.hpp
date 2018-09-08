//
// Created by Evan Kirkiles on 8/27/18.
//

#ifndef SHOPIFY_BOT_CHECKOUT_HPP
#define SHOPIFY_BOT_CHECKOUT_HPP

#ifndef string
#include <string>
#endif
#ifndef ifstream
#include <fstream>
#include <iostream>
#endif
#ifndef put_time
#include <iomanip>
#endif
#ifndef chrono
#include <chrono>
#endif
#include <random>

// cURL include
#ifndef curl
#include <curl/curl.h>
#endif

// Qt includes are already guarded
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include <QObject>
#include <QApplication>

// Include the global constants
#include "constants.hpp"

// The callback function which allows one to write data directly to a string from a libcurl request
size_t string_call_back(void *contents, size_t size, size_t nmemb, std::string *s);

// Class containing all checkout related functions in the form of a "Checkout" class
// Will be able to run multiple times until success is had.
class Checkout : public QObject {
    Q_OBJECT
public:
    // Builds the checkout with the given URL for the given profile, proxy, etc.
    explicit Checkout(const std::string& cartLink, const std::string& logFileLocation,
                      const std::string& profile, const std::string& proxy,
                      const std::string& identifier);

    // Prepares a checkout
    void prepare(const std::string& URL);

    // Runs the checkout
    void run(const std::string& URL);

signals:
    // Sets the status of the task widget
    void setStatus(QString status, QString color);

private:
    // Instances of parameters passed in
    const std::string cartLink, logFileLocation, identifier;
    std::string proxy, proxyunp;
    QJsonObject profile, ccard;

    // Required for running of the task
    std::string checkoutURL;
    std::string paymentGateway;
    std::string shippingMethod = "shopify-UPS%20GROUND%20(5-7%20business%20days)-10.00";
    std::string creditCardInstance;
    std::string totalprice;

    // Must be true for run
    bool allInitialized = false;
    bool allPrepared = false;
    bool shouldcontinue = true;

    // Cookie format
    char nline[256];

    // Logs a line to the given log file
    void log(const std::string& message);

    // Each step of the checkout is run in a function to allow for concurrency, passed in the curl instance
    void getCheckoutURL(const std::string& URL);
    void postContactInfo();
    void postShippingMethod();
    void submitCreditCard();
    void postCreditCard();
};

#endif //SHOPIFY_BOT_CHECKOUT_HPP
