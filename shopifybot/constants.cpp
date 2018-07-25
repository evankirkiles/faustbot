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
    const char* TASK_LOG = "./shopifybot/Logs/";

    // Images
    const char* LOGO_FILE = "./shopifybot/Graphics/Assets/logo.png";
    const char* PLAY_IMG = "./shopifybot/Graphics/Assets/play.png";
    const char* PLAY2_IMG = "./shopifybot/Graphics/Assets/play2.png";
    const char* STOP_IMG = "./shopifybot/Graphics/Assets/stop.png";
    const char* STOP2_IMG = "./shopifybot/Graphics/Assets/stop2.png";
    const char* EDIT_IMG = "./shopifybot/Graphics/Assets/edit.png";
    const char* EDIT2_IMG = "./shopifybot/Graphics/Assets/edit2.png";
    const char* PLAY_DISBL_IMG = "./shopifybot/Graphics/Assets/play-disabled.png";
    const char* REPLAY_IMG = "./shopifybot/Graphics/Assets/replay.png";
    const char* REPLAY2_IMG = "./shopifybot/Graphics/Assets/replay2.png";
    const char* DELETE_IMG = "./shopifybot/Graphics/Assets/delete.png";
    const char* DELETE2_IMG = "./shopifybot/Graphics/Assets/delete2.png";
    const char* LOGS_IMG = "./shopifybot/Graphics/Assets/logs.png";
    const char* LOGS2_IMG = "./shopifybot/Graphics/Assets/logs2.png";
    const char* EXIT_IMG = "./shopifybot/Graphics/Assets/exit.png";
    const char* EXIT2_IMG = "./shopifybot/Graphics/Assets/exit2.png";
    const char* MINIM_IMG = "./shopifybot/Graphics/Assets/minim.png";
    const char* MINIM2_IMG = "./shopifybot/Graphics/Assets/minim2.png";
}

namespace constants {
    const unsigned int BASE_FREQ = 30;
    const unsigned int BASE_NUMRESULTS = 3;
}


