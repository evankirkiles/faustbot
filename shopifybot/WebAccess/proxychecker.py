import requests, sys, json

# Python script which checks the functionality of the proxy given in the args
# First argument is the index of the proxy to check, and the second is the filename to write to
proxyIndex = sys.argv[1]
fileToWriteTo = sys.argv[2]

# First task is to retrieve the proxy information from the proxy file
j = None
with open('shopifybot/Infrastructure/proxies.txt') as inputfile:
    for i, line in enumerate(inputfile):
        if line.split(' :-: ', 1)[0] == proxyIndex:
            j = json.loads(line.split(' :-: ', 1)[1])
    inputfile.close()

# With the JSON information of the proxy in hand, proceed to check proxy status by doing a simple Google check
# If there is no username or password, do not use headers
