//
// Created by Evan Kirkiles on 9/2/18.
//

#ifndef FAUST_ADDRESSGENERATOR_HPP
#define FAUST_ADDRESSGENERATOR_HPP

#ifdef WIN32
// Windows specific header
#include <windows.h>
#include <intrin.h>
#include <iphlpapi.h>
#elif defined __APPLE__

#endif

#ifndef algorithm
#include <algorithm>
#endif
#ifndef string
#include <string>
#endif
#ifndef cout
#include <iostream>
#endif

// Class which generates the unique address for a computer
class Address {
public:
    // Builds the unique address
    explicit Address() = default;

    // The string representation
    std::string value;

    // Functions to smear and unsmear the string so it isn't easily reverse engineered
    static void smear(uint16_t* id);
    static void unsmear(uint16_t* id);

    // Gets a human readable version of the system id
    static std::string getSystemUniqueId();

    // Validates an ID by checking against the current system's ID
    static bool validate(const std::string& testIdString);
private:
    // Static int mask
    static constexpr uint16_t mask[5] = { 0x4e25, 0xf4a1, 0x5437, 0xab41, 0x0000 };

    // Functions to get the address components
    static uint16_t* computeSystemUniqueId();
    static void getMacHash(uint16_t& mac1, uint16_t& mac2);
    static uint16_t getVolumeHash();
    static uint16_t getCpuHash();
    static const char* getMachineName();
};


#endif //FAUST_ADDRESSGENERATOR_HPP
