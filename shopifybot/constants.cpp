//
// Created by Evan Kirkiles on 7/14/18.
//

#include "constants.hpp"

namespace supported_sites {
    const URLAndMethod SOCIALSTATUS = {"https://www.socialstatuspgh.com", 101};
    const URLAndMethod XHIBITION = {"https://www.xhibition.co", 102};
    const URLAndMethod UNDEFEATED = {"https://undefeated.com", 103};
    const URLAndMethod BLENDS = {"https://blendsus.com", 104};
    const URLAndMethod WISHATL = {"https://wishatl.com", 105};

    const URLAndMethod KITH = {"https://kith.com", 201};

    const URLAndMethod BODEGA = {"https://shop.bdgastore.com", 301};
}

namespace file_paths {
    const char* HTML_BODY = "./shopifybot/WebAccess/Contents/html_body.txt";
    const char* PRODUCTS_LOG = "./shopifybot/WebAccess/Contents/products_log.txt";

    const char* TIME_LOG = "./shopifybot/logs.txt";
    const char* TASK_LOG = "./shopifybot/task_logs.txt";
}

namespace constants {
    const unsigned int BASE_FREQ = 30;
    const unsigned int BASE_NUMRESULTS = 3;
}


