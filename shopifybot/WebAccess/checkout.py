# -*- coding: utf-8 -*-
# Imports
import requests, sys, json, datetime, random
from bs4 import BeautifulSoup


# Custom write function
def log(text):
    with open(logFileLocation, 'a') as logFile:
        logFile.write('[' + str(datetime.datetime.now())[0:23] + '] ' + text + '\n')


# First, must denote the checkout link for the Shopify server for the site
checkoutLink = sys.argv[1]
# Also need the cart link for the desired product
cartLink = sys.argv[2]
# Log file location is the third argument
logFileLocation = sys.argv[3]
# Name of the profile is the fourth argument
profileName = sys.argv[4]
# Finally, the proxy identifier is the fifth argument
proxyName = sys.argv[5]

# Go into the profile file and find the correct profile's json data
j = None
with open('shopifybot/Infrastructure/profiles.txt') as input_file:
    for i, line in enumerate(input_file):
        if line.split(' :-: ', 1)[0] == profileName:
            j = json.loads(line.split(' :-: ', 1)[1])
            break
    input_file.close()
if j is None:
    # If it gets here (j is undefined), then profile has not been found
    raise ValueError("Could not locate profile in profiles.txt")

# Go into the credit card file and find the correct credit card json data
c = None
with open('shopifybot/Infrastructure/readme.txt') as ccinput_file:
    fileLine = -1
    if j['ccard'] == 'Random':
        fileLine = 0
        for i, line in enumerate(ccinput_file):
            fileLine += 1
        fileLine = random.randint(1, fileLine + 1)

    currentLine = 0
    for i, line in enumerate(ccinput_file):
        currentLine += 1
        if line.split(' :-: ', 1)[0] == j['ccard'] or currentLine == fileLine:
            c = json.loads(line.split(' :-: ', 1)[1])
            break
    ccinput_file.close()
if c is None:
    # If it gets here (c is undefined), then the credit card has not been found
    raise ValueError("Could not locate credit card in readme.txt")

# Go into the proxy file and find the correct proxy json data
p = None
proxyDict = None
with open('shopifybot/Infrastructure/proxies.txt') as proxyinputfile:
    fileLine = -1
    if proxyName == 'Random':
        fileLine = 0
        for i, line in enumerate(proxyinputfile):
            fileLine += 1
        fileLine = random.randint(1, fileLine + 1)

    currentLine = 0
    for i, line in enumerate(proxyinputfile):
        currentLine += 1
        if line.split(' :-: ', 1)[0] == proxyName or currentLine == fileLine:
            p = json.loads(line.split(' :-: ', 1)[1])
            break
    proxyinputfile.close()
if p is not None:
    # Now build the proxy data package to send to the servers
    if p['proxyusername'] == '' or p['proxypassword'] == '':
        proxyDict = {'http': 'http://' + p['proxyip'] + ':' + p['proxyport'],
                     'https': 'https://' + p['proxyip'] + ':' + p['proxyport']}
    else:
        proxyDict = {'http': 'http://' + p['proxyip'] + ':' + p['proxyport'],
                     'https': 'https://' + p['proxyusername'] + ':' + p['proxypassword'] + '@' + p['proxyip'] + ':' + p['proxyport']}
else:
    proxyDict = {'http': '', 'https': ''}

# Create requests session
session = requests.session()


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
        resp = session.get(cartLink, allow_redirects=True, headers=customerInfoHeaders, proxies=proxyDict, timeout=4)
    except Exception as e:
        log(e)

    # Check progress of response
    if b'Continue to shipping method' in resp.content:
        log('Got to customer information form.')
    elif b'Inventory issues' in resp.content:
        log('Could not fill complete order, reordering to get as many as possible.')

        # Reformat to fill as much of the order as possible
        soup = BeautifulSoup(resp.content, 'html.parser')
        newQuantity = soup.findAll('input', {'name': 'checkout[line_items][0][quantity]'})[1]['value']
        newURL = cartLink.split(':')[0] + ':' + cartLink.split(':')[1] + ':' + newQuantity

        # Log the reformatted order
        log('Reordered for max of ' + newQuantity + ' products. New link: ' + newURL)

        # This should always go through because first one did
        try:
            resp = session.get(newURL, allow_redirects=True, headers=customerInfoHeaders, proxies=proxyDict, timeout=4)
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
    global shopifyCheckoutLink, resp
    shopifyCheckoutLink = resp.url

    # Parse through the html contents and find authenticity tokens
    soup = BeautifulSoup(resp.content, 'html.parser')
    authenticity_token = soup.findAll('input', {'name': 'authenticity_token'})[2]['value']

    # Customer data packet to send with POST HTTP request
    customerData = {
        'utf8': '✓',
        '_method': 'patch',
        'authenticity_token': authenticity_token,
        'previous_step': 'contact_information',
        'step': 'shipping_method',
        'checkout[email]': j['email'],
        'checkout[buyer_accepts_marketing]': '0',
        'checkout[shipping_address][first_name]': j['firstname'],
        'checkout[shipping_address][last_name]': j['lastname'],
        'checkout[shipping_address][address1]': j['address1'],
        'checkout[shipping_address][address2]': j['address2'],
        'checkout[shipping_address][city]': j['city'],
        'checkout[shipping_address][country]': j['country'],
        'checkout[shipping_address][province]': j['province'],
        'checkout[shipping_address][zip]': j['zipcode'],
        'checkout[shipping_address][phone]': j['phone'],
        'button': '',
        'checkout[client_details][browser_width]': '520',
        'checkout[client_details][browser_height]': '704',
        'checkout[client_details][javascript_enabled]': '1'
    }

    # Try to send this customer information in a POST request
    try:
        resp = session.post(shopifyCheckoutLink, data=customerData, proxies=proxyDict, allow_redirects=True, timeout=4)
    except Exception as e:
        log(e)

    # If the response code is 200, successfully submitted customer info
    if resp.status_code == 200:
        log('Successfully submitted customer info to Shopify server.')
    else:
        log(str(resp.status_code))

    # Now try to access the checkout link asking for the payment method
    resp = session.get(shopifyCheckoutLink + "?previous_step=shipping_method&step=payment_method", proxies=proxyDict,
                       allow_redirects=True, timeout=4)

    # Check if successfully got to payment page
    if b'Complete order' or b'Complete order'.upper in resp.content:
        log('Successfully bypassed reCAPTCHA and got to payment method page.')


# Function to go through with the payment method
def submitpayment():
    # Define payment headers
    global resp
    submitPaymentHeaders = {
        'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/67.0.3396.99 Safari/537.36',
        'Content-Type': 'application/json'
    }

    # Also define the credit card info data packet to send to Shopify
    ccInfo = {
        'credit_card': {
            'number': c['ccnum'],
            'name': c['ccname'],
            'month': c['ccmonth'],
            'year': c['ccyear'],
            'verification_value': c['ccccv']
        }
    }

    # Now try sending the post request to the Shopify Servers
    try:
        resp = session.post('https://elb.deposit.shopifycs.com/sessions', data=json.dumps(ccInfo), proxies=proxyDict,
                            headers=submitPaymentHeaders, allow_redirects=True, timeout=4)
    except Exception as e:
        log(e)

    # Check if the post request was successful
    if resp.status_code == 200:
        log('Got unique session ID from Shopify.')
    else:
        log(str(resp.status_code))


# Run these functions
send_customer_info()
submitpayment()
