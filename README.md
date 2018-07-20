# Shopify Bot

Status: **Under Development**

Bot for various supported Shopify sites. Runs 'tasks' which can be put onto different threads to allow for multithreading, each one given a keyword to search for as well as a color and a size. As soon as a product is found matching all three fields, it is purchased through a Selenium headless web browser with PayPal (Using normal credit card often requires reCAPTCHA, which takes upwards of 30 seconds and isn't efficient. You might get lucky and not run into a reCAPTCHA, but it's not a good risk to take consistently.)

**Features:**
- C++ cURL webscraper with Python used for checking out a product once variant id has been found.
- Scraping of various Shopify websites to produce list of variant ids for products' respective colors and sizes.
- Variant ID retrieval directly from a product's page given the color and size you want
- Selenium-based checkout module using PayPal (_recommended_) or normal Credit Card information.
  * PayPal checkout has not been fully tested, but should be able to be made faster with one-click checkout.
- Logging while tasks are running to keep you updated on timestamps of what the bot has done.

**To-Do List:**
- Add a GUI.
- Finish multithreading support (make tasks completely thread-safe by not sharing webscraping files).
- Try it out on a real product.

---
Contact me if you have a Shopify site you want to be supported!

**Supported Sites:**
- Blends
- Bodega
- Kith
- Social Status
- Undefeated
- WishATL
- Xhibition