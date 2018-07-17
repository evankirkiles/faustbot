from selenium import webdriver
import sys

driver = webdriver.Chrome()
driver.get(sys.argv[1])
