from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.by import By
import sys

# Set up the Selenium instance with Chrome
driver = webdriver.Chrome()
# Go to the link given as the first argument
driver.get(sys.argv[1])
# Bring the Selenium window to front
driver.switch_to.window(driver.current_window_handle)

driver.find_element()