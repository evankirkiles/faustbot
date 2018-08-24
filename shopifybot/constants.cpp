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

    const char* HTML_BODY = "/Resources/Contents/html_body_";
    const char* PRODUCTS_LOG = "/Resources/Contents/products_log_";
    const char* PROXYCHECK = "/Resources/Contents/proxychecker";

    const char* CHECKOUT = "/Resources/checkout.py";
    const char* PROXYCHECKER = "/Resources/proxychecker.py";

    const char* TIME_LOG = "/Logs/timelogs.log";
    const char* TASKS_LOG = "/Logs/task_logs_";
    const char* TASK_LOG = "/Logs/";

    const char* STYLESHEET = ":Stylesheets/stylesheet.qss";
    const char* COLORSTYLESHEET = "/Resources/Stylesheets/colorstylesheet.qss";
    const char* COLORSTYLESHEET_WITHVARS = ":Stylesheets/colorstylesheet_withvars.qss";
    const char* COLORSTYLESHEET_DEFAULT = "/Resources/Stylesheets/colorstylesheet_default.qss";

    const char* PROFILES_TXT = "/Resources/Cache/profiles.txt";
    const char* TEMPPROFILES_TXT = "/Resources/Cache/tempprofiles.txt";
    const char* CCARD_TXT = "/Resources/Cache/ccards.txt";
    const char* TEMPCCARD_TXT = "/Resources/Cache/tempccards.txt";
    const char* PROXIES_TXT = "/Resources/Cache/proxies.txt";
    const char* TEMPPROXIES_TXT = "/Resources/Cache/tempproxies.txt";
    const char* COOKIES_TXT = "/Resources/Contents/CookieJar/cookies.txt";

    const char* CONTENTS_DIR = "/Resources/Contents";
    const char* LOGS_DIR = "/Logs";

    // Images
    const char* LOGO_FILE = ":Assets/logo.png";
    const char* PLAY_IMG = ":Assets/play.png";
    const char* PLAY2_IMG = ":Assets/play2.png";
    const char* STOP_IMG = ":Assets/stop.png";
    const char* STOP2_IMG = ":Assets/stop2.png";
    const char* EDIT_IMG = ":Assets/edit.png";
    const char* EDIT2_IMG = ":Assets/edit2.png";
    const char* PLAY_DISBL_IMG = ":Assets/play-disabled.png";
    const char* REPLAY_IMG = ":Assets/replay.png";
    const char* REPLAY2_IMG = ":Assets/replay2.png";
    const char* DELETE_IMG = ":Assets/delete.png";
    const char* DELETE2_IMG = ":Assets/delete2.png";
    const char* LOGS_IMG = ":Assets/logs.png";
    const char* LOGS2_IMG = ":Assets/logs2.png";
    const char* EXIT_IMG = ":Assets/exit.png";
    const char* EXIT2_IMG = ":Assets/exit2.png";
    const char* MOREINFO_IMG = ":Assets/moreinfo.png";
    const char* MOREINFO2_IMG = ":Assets/moreinfo2.png";
    const char* ADD_IMG = ":Assets/add.png";
    const char* ADD2_IMG = ":Assets/add2.png";
    const char* MINUS_IMG = ":Assets/minus.png";
    const char* MINUS2_IMG = ":Assets/minus2.png";
    const char* DUPLICATE_IMG = ":Assets/duplicate.png";
    const char* DUPLICATE2_IMG = ":Assets/duplicate2.png";
    const char* REFRESH_IMG = ":Assets/refresh.png";
    const char* REFRESH2_IMG = ":Assets/refresh2.png";
    const char* DISABLEDREFRESH_IMG = ":Assets/disabledrefresh.png";
    const char* PROXYON_IMG = ":Assets/proxyon.png";
    const char* PROXYOFF_IMG = ":Assets/proxyoff.png";
    const char* PROXYNEUTRAL_IMG = ":Assets/proxyneutral.png";
    const char* REFRESHICON_IMG = ":Assets/refreshicon.png";
    const char* REFRESHICON2_IMG = ":Assets/refreshicon2.png";
    const char* DEFAULTTHUMB_IMG = ":Assets/defaultthumb.png";
    const char* GEAR_IMG = ":Assets/gear.png";
}

namespace constants {
    const unsigned int BASE_FREQ = 5;
    // TODO: Make a base numresults checker
    const unsigned int BASE_NUMRESULTS = 10;

    const std::map<std::string, std::string> CUSTOMIZEVARS = {
            {"Background 1", "MAINBG"}, {"Background 2", "SECONDARYBG"}, {"Font", "FONT"}, {"Border", "BORDER"},
            {"Text 1", "MAINTEXT"}, {"Text 2", "SECONDARYTEXT"}, {"Task ID", "TASKID"},
            {"Accents 1", "MAINACCENT"}, {"Accents 2", "SECONDARYACCENT"}, {"Button 1", "MAINBUTTON"},
            {"Button 1 -PRESSED-", "MAINBUTTONPRESSED"}, {"Button 2", "SIDEBUTTON"},
            {"Button 2 -PRESSED-", "SIDEBUTTONPRESSED"}, {"Button 3", "SECONDARYBUTTON"},
            {"Button 3 -PRESSED-", "SECONDARYBUTTONPRESSED"}, {"Button 4", "TERTIARYBUTTON"},
            {"Button 4 -PRESSED", "TERTIARYBUTTONPRESSED"}, {"Title Bar", "TITLEBARBG"}
    };
}


