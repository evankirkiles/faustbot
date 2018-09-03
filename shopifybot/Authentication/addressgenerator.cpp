//
// Created by Evan Kirkiles on 9/2/18.
//

#include "addressgenerator.hpp"

// For Windows, retrieve the unique hash address differently
#ifdef WIN32
// Function which gets several of the mac addresses of a computer
uint16_t hashMacAddress(PIP_ADAPTER_INFO info) {
    uint16_t hash = 0;
    for (uint32_t i = 0; i < info->AddressLength; i++) {
        hash += (info->Address[i] << ((i & 1) * 8));
    }
    return hash;
}
// Function which gets the mac address from the first two of the compiled mac address list
void Address::getMacHash(uint16_t& mac1, uint16_t&mac2) {
    IP_ADAPTER_INFO AdapterInfo[32];
    DWORD dwBufLen = sizeof(AdapterInfo);

    // Make sure the adapter can give a name
    DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
    if (dwStatus != ERROR_SUCCESS) { return; }

    PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
    mac1 = hashMacAddress(pAdapterInfo);
    if (pAdapterInfo->Next) {
        mac2 = hashMacAddress(pAdapterInfo->Next);
    }

    // Sort the mac addresses so do not invalidate the mac addresses if they change order.
    if (mac1 > mac2) {
        uint16_t tmp = mac2;
        mac2 = mac1;
        mac1 = tmp;
    }
}
// Gets the volume hash of the computer
uint16_t Address::getVolumeHash() {
    DWORD serialNum = 0;

    // Determine if the volume uses an NSTFS file system
    GetVolumeInformation("c:\\", NULL, 0, &serialNum, NULL, NULL, NULL, 0);
    uint16_t hash = (uint16_t)((serialNum + (serialNum >> 16)) & 0xFFFF);

    return hash;
}
// Gets the CPU hash of the computer
uint16_t Address::getCpuHash() {
    int cpuinfo[4] = {0, 0, 0, 0};
    __cpuid(cpuinfo, 0);
    uint16_t hash = 0;
    uint16_t* ptr = (uint16_t*)(&cpuinfo[0]);
    for (uint32_t i = 0; i < 8; i++) {
        hash += ptr[i];
    }

    return hash;
}
// Gets the Machine Name of the computer
const char* Address::getMachineName() {
    static char computerName[1024];
    DWORD size = 1024;
    GetComputerName(computerName, &size);
    return &(computerName[0]);
}

// For MacOSX
#elif defined __APPLE__
#include <unistd.h>
#include <cerrno>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if_dl.h>
#include <ifaddrs.h>
#include <net/if_types.h>
#include <mach-o/arch.h>

#include <sys/resource.h>
#include <sys/utsname.h>

// get MAC addresses
uint16_t hashMacAddress(const uint8_t* mac) {
    uint16_t hash = 0;
    for (uint32_t i = 0; i < 6; i++) {
        hash += (mac[i] << ((i & 1) * 8));
    }
    return hash;
}

// Get the MAC Address from the list of unique mac hash addresses
void Address::getMacHash(uint16_t& mac1, uint16_t& mac2) {
    mac1 = 0;
    mac2 = 0;

    // This section is Mac OSX specific
    // Get the network interface addresses
    struct ifaddrs* ifaphead;
    if (getifaddrs(&ifaphead) != 0) { return; }

    // Iterate over the network interfaces
    bool foundMac1 = false;
    struct ifaddrs* ifap;
    for (ifap = ifaphead; ifap; ifap = ifap->ifa_next) {
        auto sdl = (struct sockaddr_dl*)ifap->ifa_addr;
        if (sdl && (sdl->sdl_family == AF_LINK) && (sdl->sdl_type == IFT_ETHER)) {
            if (!foundMac1) {
                foundMac1 = true;
                mac1 = hashMacAddress((uint8_t*)(LLADDR(sdl)));
            } else {
                mac2 = hashMacAddress((uint8_t*)(LLADDR(sdl)));
                break;
            }
        }
    }

    // Now free the network interfaces
    freeifaddrs(ifaphead);

    // Sort the mac addresses so they do not invalidate the computer if they change order
    if (mac1 > mac2) {
        uint16_t tmp = mac2;
        mac2 = mac1;
        mac1 = tmp;
    }
}

// Gets the system name instead of the volume serial number there is on windows
uint16_t Address::getVolumeHash() {
    auto sysname = (uint8_t*)getMachineName();
    uint16_t hash = 0;

    // Iterate through the system name
    for (uint32_t i = 0; sysname[i]; i++) {
        hash += (sysname[i] << ((i & 1) * 8));
    }
    return hash;
}

// Get the CPU hash of the mac
uint16_t Address::getCpuHash() {
    const NXArchInfo* info = NXGetLocalArchInfo();
    uint16_t val = 0;
    val += (uint16_t)info->cputype;
    val += (uint16_t)info->cpusubtype;
    return val;
}

// Finally get the machine name
const char* Address::getMachineName() {
    static struct utsname u;
    if (uname(&u) < 0) {
        assert(0);
        return "unknown";
    }
    return u.nodename;
}

#endif

// System-regardless function implementations
// Function which smears a 16 bit int hash
void Address::smear(uint16_t* id) {
    for (uint32_t i = 0; i < 5; i++)
        for (uint32_t j = i; j < 5; j++)
            if (i != j)
                id[i] ^= id[j];

    for (uint32_t i = 0; i < 5; i++)
        id[i] ^= mask[i];
};
// Function which unsmears a smeared 16 bit int hash
void Address::unsmear(uint16_t *id) {
    for (uint32_t i = 0; i < 5; i++)
        id[i] ^= mask[i];

    for (uint32_t i = 0; i < 5; i++)
        for (uint32_t j = 0; j < i; j++)
            if (i != j)
                id[4-i] ^= id[4-j];
}

// Combines all the functions which retrieve system information to produce a unique 72 bit ID
// Combines CPU id, volume id, the first two MAC addresses, and 16 bits of check digits
uint16_t* Address::computeSystemUniqueId() {
    static uint16_t id[5];
    static bool computed = false;

    if (computed) return id;

    // Produce a number that uniquely identifies this system
    id[0] = getCpuHash();
    id[1] = getVolumeHash();
    getMacHash(id[2], id[3]);

    // Fifth block is some check digits
    id[4] = 0;
    for (uint32_t i = 0; i < 4; i++) {
        id[4] += id[i];
    }

    // Smear the id and return it
    smear(id);
    computed = true;
    return id;
}

// Gets a UNIQUE system Id in human readable format
std::string Address::getSystemUniqueId() {
    // Get the name of the computer
    std::string buf;
    buf.append(getMachineName());
    // Remove -'s so that the delimiters do not mess with the encoding
    buf.erase(std::remove (buf.begin(), buf.end(), '-'), buf.end());


    // Now make the rest of the ID
    uint16_t* id = computeSystemUniqueId();
    for (uint32_t i = 0; i < 5; i++) {
        char num[16];
        snprintf(num, 16, "%x", id[i]);
        buf.append("-");
        switch(strlen(num)) {
            case 1: buf.append("000"); break;
            case 2: buf.append("00"); break;
            case 3: buf.append("0"); break;
            default: break;
        }
        buf.append(num);
    }

    // Now return the string, putting it to uppercase
    std::transform(buf.begin(), buf.end(), buf.begin(), ::toupper);
    return buf;
}

// Validate the ID against the system's fingerprint
bool Address::validate(const std::string& testIdString) {
    // Unpack the given string, making sure that it can unpack the string
    char* testName = strtok(const_cast<char *>(testIdString.c_str()), "-");
    if (!testName) return false;
    uint16_t testId[5];
    for (uint32_t i=0; i < 5; i++) {
        char* testNum = strtok( NULL, "-" );
        if ( !testNum ) return false;
        testId[i] = (uint16_t)(strtol( testNum, NULL, 16 ));
    }
    // Unsmear the id as well
    unsmear(testId);

    // Make sure the ID is valid by comparing it to the checkdigits
    uint16_t check = 0;
    for (uint32_t i = 0; i < 4; i++) {
        check += testId[i];
    }
    if (check != testId[4]) return false;

    // Now get the current system information
    uint16_t systemId[5];
    memcpy(systemId, computeSystemUniqueId(), sizeof(systemId));
    unsmear(systemId);

    // Now score the match
    uint32_t score = 0;
    for (uint32_t i = 0; i < 4; i++)
        if (testId[i] == systemId[i])
            score++;
    if (!strcmp(getMachineName(), testName))
        score++;

    // If there are 3 points or more then the id matches
    return score >= 3;

}
