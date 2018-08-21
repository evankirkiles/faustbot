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
    const URLAndMethod HANON = {"https://www.hanon-shop.com", "hanon-shop.com", 106, "hanon"};
    const URLAndMethod SHOEGALLERY = {"https://shoegallerymiami.com", "shoegallerymiami.com", 107, "shoegallery"};
    const URLAndMethod SHOPNICEKICKS = {"https://shopnicekicks.com", "shopnicekicks.com", 108, "shopnicekicks"};
    const URLAndMethod NOTRESHOP = {"https://www.notre-shop.com", "notre-shop.com", 109, "notreshop"};
    const URLAndMethod TWELVEAMRUN = {"https://www.12amrun.com", "12amrun.com", 110, "12amrun"};
    const URLAndMethod EIGHTEENMONTROSE = {"https://18montrose.com", "18montrose.com", 111, "18montrose"};
    const URLAndMethod AMAMANIERE = {"https://www.a-ma-maniere.com", "a-ma-maniere.com", 112, "a-ma-maniere"};
    const URLAndMethod APBSTORE = {"https://www.apbstore.com", "apbstore.com", 113, "apbstore"};
    const URLAndMethod ADDICTMIAMI = {"https://www.addictmiami.com", "addictmiami.com", 114, "addictmiami"};
    const URLAndMethod ASSC = {"https://shop.antisocialsocialclub.com", "shop.antisocialsocialclub.com", 115, "assc"};
    const URLAndMethod EXCLUSIVEDNA = {"https://www.exclusivedna.com", "exclusivedna.com", 116, "exclusivedna"};
    const URLAndMethod BBCICECREAM = {"https://www.bbcicecream.com", "bbcicream.com", 117, "billionaireboysclub"};
    const URLAndMethod BAPE = {"https://bapeonline.com", "bapeonline.com", 118, "bape"};
    const URLAndMethod BEATNIC = {"https://beatniconline.com", "beatniconline.com", 119, "beatnic"};
    const URLAndMethod BLACKMARKETUS = {"https://www.blkmkt.us", "blkmkt.us", 120, "blkmkt"};

    const URLAndMethod KITH = {"https://kith.com", "kith.com", 201, "kith"};

    const URLAndMethod BODEGA = {"https://shop.bdgastore.com", "shop.bdgastore.com", 301, "bodega"};

    const std::unordered_map<std::string, URLAndMethod>WEBSITES =
            {{"Social Status", SOCIALSTATUS}, {"Xhibition", XHIBITION}, {"Undefeated", UNDEFEATED},
             {"Blends", BLENDS}, {"WishATL", WISHATL}, {"Kith", KITH}, {"Bodega", BODEGA},
             {"Hanon", HANON}, {"Shoe Gallery", SHOEGALLERY}, {"ShopNiceKicks", SHOPNICEKICKS},
             {"Notre", NOTRESHOP}, {"12amrun", TWELVEAMRUN}, {"18montrose", EIGHTEENMONTROSE},
             {"A Ma Maniére", AMAMANIERE}, {"APB Store", APBSTORE}, {"Addict", ADDICTMIAMI},
             {"Anti Social Social Club", ASSC}, {"Exclusive DNA", EXCLUSIVEDNA}, {"Billionaire Boys Club", BBCICECREAM},
             {"BAPE", BAPE}, {"BEATNIC", BEATNIC}, {"Black Market USA", BLACKMARKETUS}};
    const std::unordered_map<std::string, std::string>WEBSITES_BWD =
            {{SOCIALSTATUS.baseURL, "Social Status"}, {XHIBITION.baseURL, "Xhibition"}, {UNDEFEATED.baseURL, "Undefeated"},
             {BLENDS.baseURL, "Blends"}, {WISHATL.baseURL, "WishATL"}, {KITH.baseURL, "Kith"}, {BODEGA.baseURL, "Bodega"},
             {HANON.baseURL, "Hanon"}, {SHOEGALLERY.baseURL, "Shoe Gallery"}, {SHOPNICEKICKS.baseURL, "ShopNiceKicks"},
             {NOTRESHOP.baseURL, "Notre"}, {TWELVEAMRUN.baseURL, "12amrun"}, {EIGHTEENMONTROSE.baseURL, "18montrose"},
             {AMAMANIERE.baseURL, "A Ma Maniére"}, {APBSTORE.baseURL, "APB Store"}, {ADDICTMIAMI.baseURL, "Addict"},
             {ASSC.baseURL, "Anti Social Social Club"}, {EXCLUSIVEDNA.baseURL, "Exclusive DNA"},
             {BBCICECREAM.baseURL, "Billionaire Boys Club"}, {BAPE.baseURL, "BAPE"}, {BEATNIC.baseURL, "BEATNIC"},
             {BLACKMARKETUS.baseURL, "Black Market USA"}};

    // BEATNIC CURRENTLY UNSUPPORTED, ADD IN WHEN FIXED
    const QStringList ssStringList({"12amrun", "18montrose", "A Ma Maniére", "Addict", "Anti Social Social Club",
                                    "APB Store", "BAPE", "Billionaire Boys Club", "Black Market USA", "Blends", "Bodega",
                                    "Exclusive DNA", "Hanon", "Kith", "Notre", "Shoe Gallery", "ShopNiceKicks",
                                    "Social Status", "Undefeated", "WishATL", "Xhibition"});

}

namespace file_paths {
    const char* HTML_BODY = "./shopifybot/WebAccess/Contents/html_body_";
    const char* PRODUCTS_LOG = "./shopifybot/WebAccess/Contents/products_log_";

    const char* TIME_LOG = "./shopifybot/logs.txt";
    const char* TASK_LOG = "./shopifybot/Infrastructure/Logs/";

    const char* PROFILES_TXT = "./shopifybot/Infrastructure/profiles.txt";
    const char* TEMPPROFILES_TXT = "./shopifybot/Infrastructure/tempprofiles.txt";
    const char* CCARD_TXT = "./shopifybot/Infrastructure/readme.txt";
    const char* TEMPCCARD_TXT = "./shopifybot/Infrastructure/tempreadme.txt";
    const char* PROXIES_TXT = "./shopifybot/Infrastructure/proxies.txt";
    const char* TEMPPROXIES_TXT = "./shopifybot/Infrastructure/tempproxies.txt";
    const char* COOKIES_TXT = "./shopifybot/WebAccess/Contents/CookieJar/cookies.txt";

    const char* CONTENTS_DIR = "./shopifybot/WebAccess/Contents";
    const char* LOGS_DIR = "./shopifybot/Infrastructure/Logs";

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
    const char* MOREINFO_IMG = "./shopifybot/Graphics/Assets/moreinfo.png";
    const char* MOREINFO2_IMG = "./shopifybot/Graphics/Assets/moreinfo2.png";
    const char* ADD_IMG = "./shopifybot/Graphics/Assets/add.png";
    const char* ADD2_IMG = "./shopifybot/Graphics/Assets/add2.png";
    const char* MINUS_IMG = "./shopifybot/Graphics/Assets/minus.png";
    const char* MINUS2_IMG = "./shopifybot/Graphics/Assets/minus2.png";
    const char* DUPLICATE_IMG = "./shopifybot/Graphics/Assets/duplicate.png";
    const char* DUPLICATE2_IMG = "./shopifybot/Graphics/Assets/duplicate2.png";
    const char* REFRESH_IMG = "./shopifybot/Graphics/Assets/refresh.png";
    const char* REFRESH2_IMG = "./shopifybot/Graphics/Assets/refresh2.png";
    const char* DISABLEDREFRESH_IMG = "./shopifybot/Graphics/Assets/disabledrefresh.png";
    const char* PROXYON_IMG = "./shopifybot/Graphics/Assets/proxyon.png";
    const char* PROXYOFF_IMG = "./shopifybot/Graphics/Assets/proxyoff.png";
    const char* PROXYNEUTRAL_IMG = "./shopifybot/Graphics/Assets/proxyneutral.png";
    const char* REFRESHICON_IMG = "./shopifybot/Graphics/Assets/refreshicon.png";
    const char* REFRESHICON2_IMG = "./shopifybot/Graphics/Assets/refreshicon2.png";
}

namespace constants {
    const unsigned int BASE_FREQ = 5;
    // TODO: Make a base numresults checker
    const unsigned int BASE_NUMRESULTS = 10;
}


