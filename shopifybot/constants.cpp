//
// Created by Evan Kirkiles on 7/14/18.
//

#include "constants.hpp"

namespace supported_sites {
    const URLAndMethod SOCIALSTATUS = {"https://www.socialstatuspgh.com", "socialstatuspgh.com", 101, "socialstatus"};
    const URLAndMethod XHIBITION = {"https://www.xhibition.co", "xhibition.co", 102, "xhibition"};
    const URLAndMethod UNDEFEATED = {"https://undefeated.com", "undefeated.com", 103, "undefeated"};
    const URLAndMethod BLENDS = {"https://blendsus.com", "blendsus.com", 104, "blends"};
    const URLAndMethod WISHATL = {"https://wishatl.com", "wishatl.com", 105, "wish"};

    const URLAndMethod KITH = {"https://kith.com", "kith.com", 201, "kith"};

    const URLAndMethod BODEGA = {"https://shop.bdgastore.com", "shop.bdgastore.com", 301, "bodega"};
}

namespace file_paths {
    const char* HTML_BODY = "./shopifybot/WebAccess/Contents/html_body_";
    const char* PRODUCTS_LOG = "./shopifybot/WebAccess/Contents/products_log_";

    const char* TIME_LOG = "./shopifybot/logs.txt";
    const char* TASK_LOG = "./shopifybot/task_logs.txt";
}

namespace constants {
    const unsigned int BASE_FREQ = 30;
    const unsigned int BASE_NUMRESULTS = 3;
}


