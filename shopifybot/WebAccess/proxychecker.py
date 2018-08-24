import requests, sys, json, urllib3

# Python script which checks the functionality of the proxy given in the args
# First argument is the index of the proxy to check, and the second is the filename to write to
proxyIndex = sys.argv[1]
fileToWriteTo = sys.argv[2]
rootDir = sys.argv[3]

# First task is to retrieve the proxy information from the proxy file
j = None
with open(rootDir + '/Resources/Cache/proxies.txt') as inputfile:
    for i, line in enumerate(inputfile):
        if line.split(' :-: ', 1)[0] == proxyIndex:
            j = json.loads(line.split(' :-: ', 1)[1])
            break
    inputfile.close()

# With the JSON information of the proxy in hand, proceed to check proxy status by doing a simple Google check
# If there is no username or password, do not use headers
if j['proxyusername'] == '' or j['proxypassword'] == '':
    proxyDict = {'http': 'http://' + j['proxyip'] + ':' + j['proxyport'],
                 'https': 'https://' + j['proxyip'] + ':' + j['proxyport']}
# Otherwise do use headers
else:
    proxyDict = {'http': 'http://' + j['proxyusername'] + ':' + j['proxypassword'] + '@' + j['proxyip'] + ':' + j['proxyport'],
                 'https': 'https://' + j['proxyusername'] + ':' + j['proxypassword'] + '@' + j['proxyip'] + ':' + j['proxyport']}

# Then use the proxy to grab a Google webpage and make sure contains the requested data
try:
    req = requests.get('https://reddit.com', proxies=proxyDict, timeout=4)
    with open(fileToWriteTo, 'w+') as logfile:
        logfile.write('1')
        logfile.close()
except requests.exceptions.ProxyError as e:
    # In this case, the proxy simply had an error and could not connect
    with open(fileToWriteTo, 'w+') as logfile:
        logfile.write('0')
        logfile.close()
except urllib3.exceptions.LocationParseError as e:
    # In this case, there was a redirect error or some issue with the port and ip
    with open(fileToWriteTo, 'w+') as logfile:
        logfile.write('2')
        logfile.close()
except urllib3.exceptions.TimeoutError as e:
    # In this case, the proxy took too long to connect (4 second timeout)
    with open(fileToWriteTo, 'w+') as logfile:
        logfile.write('3')
        logfile.close()
