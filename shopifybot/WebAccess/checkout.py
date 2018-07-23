# -*- coding: utf-8 -*-
# Imports
import requests, sys, json, datetime
from bs4 import BeautifulSoup

# Log file location is the third argument
logFileLocation = sys.argv[3]

################################## Cart Links ##########################################
# First, must denote the checkout link for the Shopify server for the site
checkoutLink = sys.argv[1]
# Also need the cart link for the desired product
cartLink = sys.argv[2]
################################# User Settings ########################################
# Define user settings here for the checkout form
email = "sneakerbot78@gmail.com"
firstName = "Aiden"
lastName = "King"
address1 = "2824 Rosewood Lane"
address2 = ""
city = "New York"
country = "United States"
province = "New York"
zipcode = "10016"
phone = "2129539871"
# Credit card settings
ccnumber = "4621 3861 6179 4239"
ccname = "Aiden King"
ccmonth = "1"
ccyear = "2021"
ccccv = "567"
########################################################################################

# Create requests session
session = requests.session()

# Custom write function
def log(text):
    with open(logFileLocation, 'a') as logFile:
        logFile.write('[' + str(datetime.datetime.now())[0:23] + '] ' + text + '\n')

# Function that sends the customer info to the Shopify servers
def send_customer_info():

    # Headers to send with data
    customerInfoHeaders = {
        'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8',
        'Accept-Encoding': 'gzip, deflate, br',
        'Accept-Language': 'en-US,en;q=0.8',
        'Connection': 'keep-alive',
        'Content-Type': 'application/x-www-form-urlencoded',
        'Origin': checkoutLink,
        'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3396.99 Safari/537.36',
    }

    # Try to access the cart link with requests
    try:
        resp = session.get(cartLink, allow_redirects=True, headers=customerInfoHeaders, timeout=4)
    except Exception as e:
        log(e)

    # Check progress of response
    if b'Continue to shipping method' in resp.content:
        log('Got to customer information form.')
    elif b'Inventory issues' in resp.content:
        log('Could not fill complete order, reordering to get as many as possible.')

        # Reformat to fill as much of the order as possible
        soup = BeautifulSoup(resp.content, 'html.parser')
        newQuantity = soup.findAll('input', {'name':'checkout[line_items][0][quantity]'})[1]['value']
        newURL = cartLink.split(':')[0] + ':' + cartLink.split(':')[1] + ':' + newQuantity

        # Log the reformatted order
        log('Reordered for max of ' + newQuantity + ' products. New link: ' + newURL)

        # This should always go through because first one did
        try:
            resp = session.get(newURL, allow_redirects=True, headers=customerInfoHeaders, timeout=4)
        except Exception as e:
            log(e)

        if b'Continue to shipping method' in resp.content:
            log('Got to customer information form.')
        else:
            log('Customer Information page not reached. Aborting.')
            raise ValueError("Did not reach Customer Information Page!")

    else:
        log('Customer Information page not reached. Aborting.')
        raise ValueError("Did not reach Customer Information Page!")

    # Save the response URL as the Shopify checkout link
    global shopifyCheckoutLink
    shopifyCheckoutLink = resp.url

    # Parse through the html contents and find authenticity tokens
    soup = BeautifulSoup(resp.content, 'html.parser')
    authenticity_token = soup.findAll('input', {'name':'authenticity_token'})[2]['value']

    # Customer data packet to send with POST HTTP request
    customerData = {
        'utf8': '✓',
        '_method': 'patch',
        'authenticity_token': authenticity_token,
        'previous_step': 'contact_information',
        'step': 'shipping_method',
        'checkout[email]': email,
        'checkout[buyer_accepts_marketing]': '0',
        'checkout[shipping_address][first_name]': firstName,
        'checkout[shipping_address][last_name]': lastName,
        'checkout[shipping_address][address1]': address1,
        'checkout[shipping_address][address2]': address2,
        'checkout[shipping_address][city]': city,
        'checkout[shipping_address][country]': country,
        'checkout[shipping_address][province]': province,
        'checkout[shipping_address][zip]': zipcode,
        'checkout[shipping_address][phone]': phone,
        'button': '',
        'checkout[client_details][browser_width]': '520',
        'checkout[client_details][browser_height]': '704',
        'checkout[client_details][javascript_enabled]': '1'
    }

    # Try to send this customer information in a POST request
    try:
        resp = session.post(shopifyCheckoutLink, data=customerData, allow_redirects=True, timeout=4)
    except Exception as e:
        log(e)

    # If the response code is 200, successfully submitted customer info
    if resp.status_code == 200:
        log('Successfully submitted customer info to Shopify server.')
    else:
        log(str(resp.status_code))

    # Now try to access the checkout link asking for the payment method
    resp = session.get(shopifyCheckoutLink+"?previous_step=shipping_method&step=payment_method", allow_redirects=True, timeout=4)

    # Check if successfully got to payment page
    if b'Complete order' or b'Complete order'.upper in resp.content:
        log('Successfully bypassed reCAPTCHA and got to payment method page.')

# Function to go through with the payment method
def submitPayment():

    # Define payment headers
    submitPaymentHeaders = {
        'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3396.99 Safari/537.36',
        'Content-Type': 'application/json'
    }

    # Also define the credit card info data packet to send to Shopify
    ccInfo = {
        'credit_card': {
            'number': ccnumber,
            'name': ccname,
            'month': ccmonth,
            'year': ccyear,
            'verification_value': ccccv
        }
    }

    # Now try sending the post request to the Shopify Servers
    try:
        resp = session.post('https://elb.deposit.shopifycs.com/sessions', data=json.dumps(ccInfo), headers=submitPaymentHeaders, allow_redirects=True, timeout=4)
    except Exception as e:
        log(e)

    # Check if the post request was successful
    if resp.status_code == 200:
        log('Got unique session ID from Shopify.')
    else:
        log(str(resp.status_code))

# Run these functions
send_customer_info()
submitPayment()