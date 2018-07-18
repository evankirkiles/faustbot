from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.by import By
import sys
import time

################################### Checkout Details ##########################################
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

###############################################################################################

# Set up the Selenium instance with Chrome
driver = webdriver.Chrome()
# Go to the link given as the first argument
driver.get(sys.argv[1])
# Bring the Selenium window to front
driver.switch_to.window(driver.current_window_handle)

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

# User should now be completing the reCAPTCHA, so wait until the check button is pressed
while driver.find_elements(By.CLASS_NAME, 'step__step__footer__continue-btn btn ').size() == 0 :
    time.sleep(3)

