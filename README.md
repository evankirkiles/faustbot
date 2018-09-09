# Faust Bot for SHOPIFY

<img src="/screenshot1.png" alt="Screenshot of interface"/>

Status: **Dormant**

***UPDATE:*** I've stopped working on this due to school starting up and quite honestly a lack of motivation. If you have any questions, I'll try to answer them if you send me an email (found on my main github page). Feel free to use whatever code you find useful here, but please star if you do. :)

Bot for various supported Shopify sites. Runs tasks using QThreads to allow for concurrency. Checkout works as long as you don't run into captcha, although the status box for each task is very misleading. There is no system to evaluate the success of orders yet as well, but if you put in a valid credit card and the HTML body for the task after running is the processing payment page (about 31 KB), it should have worked. 65 KB HTML body file indicates that a captcha popped up.

Authentication system can be enabled by creating a MySQL database and filling in its information in constants.hpp. Then, create a new table in the database called "auth" with two columns: "auth_token" and "machine_hash". To add an authentication key, simply add a row with the given token in "auth_token" and NULL in "machine_hash". This can be done with a SQL command like so:

```
USE ${YOUR DATABASE NAME HERE};
INSERT INTO auth (auth_token,machine_hash) VALUES ('${YOUR NEW AUTH TOKEN HERE}', );
```

**Dependencies:**
- libcurl
- Qt 5 or greater (Core, Widgets, Concurrent, Network, and Sql modules)
- CPack (for building portable Mac executable)

**Platforms:**

I built this application on a Mac so I made it a portable executable using Mac-specific settings. One can run it through a compiler on a Windows computer after removing the Mac packaging code in the CMakeLists.txt, but building a Windows executable would take a little bit more work.

To build the .dmg, follow these instructions after building (in CLion) and using `cd` into the build directory:

```
cd cmake-build-debug
cpack -G DragNDrop CPackConfig.cmake
```

**Features:**
- C++ cURL webscraper and checkout.
- Scraping of various Shopify websites to produce list of variant ids for products' respective colors and sizes.
- Availability filtering of Shopify website scraping.
- Support for multiple credit cards and profiles which can be randomized for each checkout.
- Proxy support and proxy checker using cURL.
- Save/load tasks to/from JSON file.
- Start tasks at a certain time.
- Fully customizable interface.
- MySQL authentication system.

**To-Do List:**
- Try it out on a real product.
- Add "More Info Displays."
- Support sites which require logging in before checkout.

---

To add support for a website, first make sure it has a products.json page (Do this by appending /products.json to the main page or any collection page). If it does, then add it to constants file with a method number in the 100's and try parsing products from it. Ideally, the products will be formatted with their title first (after TITLE: ), then color (after COLOR:), and then their variant IDs with the size and corresponding id separated by a ':-:'.

**Supported Sites:**
- 12amrun
- 18montrose
- A Ma Maniére
- Addict
- Anti Social Social Club
- APB Store
- Bape
- Billionaire Boys' Club
- Black Market USA
- Blends
- Bodega
- Exclusive DNA
- Hanon (REQUIRES LOGIN, so not really)
- Kith
- Notre
- Shoe Gallery
- ShopNiceKicks
- Social Status
- Undefeated (REQUIRES LOGIN, so not really)
- WishATL
- Xhibition
