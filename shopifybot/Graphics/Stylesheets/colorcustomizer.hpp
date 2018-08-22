//
// Created by Evan Kirkiles on 8/22/18.
//

#ifndef SHOPIFY_BOT_COLORCUSTOMIZER_HPP
#define SHOPIFY_BOT_COLORCUSTOMIZER_HPP

// Standard includes
#ifndef ofstream
#include <fstream>
#endif

// STL includes
#ifndef unordered_map
#include <unordered_map>
#endif
#ifndef string
#include <string>
#endif

// Include the filepaths
#include "constants.hpp"

// Class which contains a custom Sass-like text editor which stores default variables and puts them into
// the color text editor in their respective positions.
class ColorCustomizer {
public:
    // Constructor simply initializes the current colors array with its values
    explicit ColorCustomizer();

    // Array containing the current colors. This should not be edited directly, but rather through the
    // edit functions which do it safely and edit the color file as well.
    std::unordered_map<std::string, std::string> currentColors;

    // MEMBER FUNCTIONS
    // Edits the specified variable in the text file and in the currentColors array.
    void changeValue(const std::string& name, const std::string& value);

    // Resets a specific setting to its default
    void resetValue(const std::string& name);
    // Resets all settings to their defaults from the default file
    void resetAllValues();

private:
    // Default variable settings
    std::unordered_map<std::string, std::string> defaultColors;
};

#endif //SHOPIFY_BOT_COLORCUSTOMIZER_HPP
