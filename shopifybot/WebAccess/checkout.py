from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.by import By
from selenium.webdriver.common.action_chains import ActionChains
import time
import os
import sys

################################### Checkout Details ##########################################
# Only thing required is PayPal information
paypalemail = "some-paypal-account"
paypalpass = "some-paypal-password"
###############################################################################################

# Set Chrome options for headless, when ready
options = webdriver.ChromeOptions()
options.add_argument('headless')

# Set up the Selenium instance
# If want to see what is happening, do not add chrome_options as a parameter
driver = webdriver.Chrome(chrome_options=options)
mainwindow = driver.current_window_handle
driver.set_window_size(200, 600, mainwindow)

# Open the website given as the first argument
driver.get(sys.argv[1])

# Click on the PayPal element
driver.find_element(By.ID, 'paypal-express-checkout-btn').click()

# Switch to the new PayPal popup window
paypalwindow = None
while not paypalwindow:
    for handle in driver.window_handles:
        if handle != mainwindow:
            paypalwindow = handle
            break
driver.switch_to.window(paypalwindow)

# Wait for PayPal to load
while len(driver.find_elements(By.ID, 'email')) == 0:
        time.sleep(1)

# Continue once PayPal has loaded
driver.find_element(By.ID, 'email').send_keys(paypalemail)
driver.find_element(By.ID, 'btnNext').click()
# Wait for email to pass
while not driver.find_element(By.ID, 'password').is_displayed():
    time.sleep(1)
driver.find_element(By.ID, 'password').send_keys(paypalpass)
driver.find_element(By.ID, 'btnLogin').click()
