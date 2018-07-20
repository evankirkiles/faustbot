//
// Created by Evan Kirkiles on 7/19/18.
//

// Google Test module
#include <gtest/gtest.h>
#include <constants.hpp>
// Class to be tested include
#include "WebAccess/product_scraper.hpp"

// Web Handler test fixture
class WebHandlerFixture : public ::testing::Test {
protected:
    void TearDown() override {}
    void SetUp() override {}
public:
    // Empty Constructor
    WebHandlerFixture() : Test() {};
    // Can change the destructor if necessary as well
    ~WebHandlerFixture() override = default;
};

// Tests whether the Web Handler successfully pulls any page information
TEST(WebHandlerFixture, getsPageData) {

    // Clear the HTML body text file first
    std::ifstream file;
    file.open(std::string(std::string(file_paths::HTML_BODY) + supported_sites::KITH.title + ".txt").c_str(), std::ios::trunc);
    file.close();

    // Instantiate a Shopify Web Handler and pull all models
    ShopifyWebsiteHandler swh(supported_sites::KITH);
    swh.getAllModels("/collections/footwear");

    // Check the file again now and make sure that it is not empty
    file.open(std::string(std::string(file_paths::HTML_BODY) + supported_sites::KITH.title + ".txt").c_str());
    EXPECT_NE(std::ifstream::traits_type::eof(), file.peek());
}

// Tests whether the Web Handler successfully pulls any product data
TEST(WebHandlerFixture, getsProductData) {

    // Clear the product data text file first
    std::ifstream file;
    file.open(std::string(std::string(file_paths::PRODUCTS_LOG) + supported_sites::KITH.title + ".txt").c_str(), std::ios::trunc);
    file.close();

    // Instantiate a Shopify Web Handler and pull all models
    ShopifyWebsiteHandler swh(supported_sites::KITH);
    swh.getAllModels("/collections/footwear");

    // Check the file again now and make sure that it is not empty
    file.open(std::string(std::string(file_paths::PRODUCTS_LOG) + supported_sites::KITH.title + ".txt").c_str());
    EXPECT_NE(std::ifstream::traits_type::eof(), file.peek());
}
