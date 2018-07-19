from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.by import By
from selenium.webdriver.common.action_chains import ActionChains
import time
import os

################################### Checkout Details ##########################################
# Google account to decrease captcha probability
googleacc = "sneakerbot78"
googlepass = "brown2018"
# Currently random address information except for email
email = "sneakerbot78@gmail.com"
first_name = "John"
last_name = "Doe"
address = "2824 Rosewood Lane"
city = "New York"
country = "United States"
state = "New York"
zip_code = "10016"
phone = "2129539871"
# Credit card information
ccnumber = "4621386161794239"
ccname = "Aiden King"
ccexpiry = "01/21"
ccccv = "567"
###############################################################################################

# Set Chrome options
# Currently UNUSED
options = webdriver.ChromeOptions()
options.add_argument('headless')

# Set up the Selenium instance with phantomJS
driver = webdriver.Chrome()
driver.set_window_size(200, 600, driver.current_window_handle)

# Log into Google to reduce captcha probabilities
driver.get("https://accounts.google.com/signin")
firstField = driver.find_element(By.ID, 'identifierId')
firstField.send_keys(googleacc)
firstField.send_keys(Keys.RETURN)
time.sleep(2)
secondField = driver.find_element(By.CLASS_NAME, 'whsOnd')
secondField.send_keys(googlepass)
secondField.send_keys(Keys.RETURN)

# Now stall until the communications file has a link in it
while os.stat("WebAccess/communications.txt").st_size == 0:
    time.sleep(1)

# Go to the link given in the communications file
communications = open("WebAccess/communications.txt", "r")
driver.get(communications.read())

# Send necessary information to the required fields
driver.find_element(By.ID, 'checkout_email').send_keys(email)
driver.find_element(By.ID, 'checkout_buyer_accepts_marketing').click()
driver.find_element(By.ID, 'checkout_shipping_address_first_name').send_keys(first_name)
driver.find_element(By.ID, 'checkout_shipping_address_last_name').send_keys(last_name)
driver.find_element(By.ID, 'checkout_shipping_address_address1').send_keys(address)
driver.find_element(By.ID, 'checkout_shipping_address_city').send_keys(city)
driver.find_element(By.ID, 'checkout_shipping_address_country').send_keys(country)
driver.find_element(By.ID, 'checkout_shipping_address_province').send_keys(state)
driver.find_element(By.ID, 'checkout_shipping_address_zip').send_keys(zip_code)
driver.find_element(By.ID, 'checkout_shipping_address_phone').send_keys(phone)

# User should now be completing the reCAPTCHA, so checks every second if past captcha page
driver.switch_to.window(driver.current_window_handle)
while len(driver.find_elements(By.ID, 'g-recaptcha')) != 0:
    time.sleep(5)

# Once python is onto the next page past the reCAPTCHA, it can resume control over the webpage
driver.find_element(By.CLASS_NAME, 'step__footer__continue-btn').click()

# Send necessary information to the required fields
# First have to switch to each frame to input to them
iframe = driver.find_elements(By.CLASS_NAME, 'card-fields-iframe')
driver.switch_to.frame(iframe[0])
driver.find_element(By.ID, 'number').send_keys(ccnumber)
driver.switch_to.default_content()
driver.switch_to.frame(iframe[1])
driver.find_element(By.ID, 'name').send_keys(ccname)
driver.switch_to.default_content()
driver.switch_to.frame(iframe[2])
driver.find_element(By.ID, 'expiry').send_keys(ccexpiry)
driver.switch_to.default_content()
driver.switch_to.frame(iframe[3])
driver.find_element(By.ID, 'verification_value').send_keys(ccccv)

# Finish checkout by clicking pay button
driver.switch_to.default_content()
driver.find_element(By.CLASS_NAME, 'step__footer__continue-btn').click()
