//
// Created by Evan Kirkiles on 8/22/18.
//

#include "colorcustomizer.hpp"

// Constructor that builds the color customizer class
ColorCustomizer::ColorCustomizer() {

    // Initialize the currentColors array from the colorstylesheet file
    std::ifstream filein(QApplication::applicationDirPath().append(file_paths::COLORSTYLESHEET).toStdString().c_str());
    std::string str;

    // Cycle through the vars comment and set the values for each index in the currentColors map
    while (getline(filein, str)) {
        // Variable names come directly after a '-'
        const unsigned long dashPos = str.find('-');
        if (dashPos != std::string::npos) {
            str.erase(0, dashPos);

            // Get an lvalue of the variable name
            const std::string varName = str.substr(1, str.find(' ') - 1);
            // Finally, set the variable's value
            str.erase(0, str.find(" :-: ") + 5);
            str.pop_back();
            currentColors[varName] = str;
            continue;
        }

        // If there is no dash, make sure that the comment has not ended
        if (str.find("*/") != std::string::npos) { break; }
    }

    // Close the file
    filein.close();

    // Also build the default colors array from the default colorstylesheet
    filein.open(QApplication::applicationDirPath().append(file_paths::COLORSTYLESHEET_DEFAULT).toStdString().c_str());

    // Cycle through the vars comment and set the values for each index in the defaultColors map
    while (getline(filein, str)) {
        // Variable names come directly after a '-'
        const unsigned long dashPos = str.find('-');
        if (dashPos != std::string::npos) {
            str.erase(0, dashPos);

            // Get an lvalue of the variable name
            const std::string varName = str.substr(1, str.find(' ') - 1);
            // Finally, set the variable's value
            str.erase(0, str.find(" :-: ") + 5);
            str.pop_back();
            defaultColors[varName] = str;
            continue;
        }

        // If there is no dash, make sure that the comment has not ended
        if (str.find("*/") != std::string::npos) { break; }
    }

    // Close the file
    filein.close();
}

// Function that changes the value of a variable in the unordered map
void ColorCustomizer::changeValue(const std::string &name, const std::string &value) {

    // Change the value in the array first
    currentColors[name] = value;

    // Now change the value in the current color stylesheet
    QFile filein(file_paths::COLORSTYLESHEET_WITHVARS);

    // Iterate through the lines of the colorstylesheet with variables
    if (filein.open(QIODevice::ReadOnly)) {
        QTextStream in(&filein);
        std::ofstream fileout(QApplication::applicationDirPath().append(file_paths::COLORSTYLESHEET).toStdString().c_str(), std::ios::trunc);
        while (!in.atEnd()) {
            std::string str = in.readLine().toStdString();
            std::string tempstr;

            // Check for the variable name signifier
            const unsigned long atPos = str.find('@');
            if (atPos != std::string::npos) {
                // When the name identifier is found, then replace it with the variable
                tempstr = str.substr(0, atPos);
                str.erase(0, atPos);
                std::string varName = str.substr(1, str.find(';') - 1);
                tempstr.append(currentColors[varName]).append(";");
            } else {
                // Otherwise just write in the line
                tempstr = str;
            }

            // Write the edited line into the color stylesheet file
            fileout << tempstr << "\n";
        }

        // Close both files
        filein.close();
        fileout.close();
    }
}

// Reset a value to its default
void ColorCustomizer::resetValue(const std::string &name) {
    // Simply change the color value to its default
    changeValue(name, defaultColors[name]);
}

// Reset all values to their defaults
void ColorCustomizer::resetAllValues() {

    // Set currentcolors to default colors
    currentColors = defaultColors;

    // Copy over the contents of the default stylesheet to that of the new one
    std::ifstream filein(QApplication::applicationDirPath().append(file_paths::COLORSTYLESHEET_DEFAULT).toStdString().c_str());
    std::ofstream fileout(QApplication::applicationDirPath().append(file_paths::COLORSTYLESHEET).toStdString().c_str(), std::ios::trunc);
    std::string str;
    while (getline(filein, str)) {
        fileout << str << "\n";
    }
    filein.close();
    fileout.close();
}