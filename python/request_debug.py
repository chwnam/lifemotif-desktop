#!/usr/bin/python
import urllib2
import json

code = raw_input("input code: ")

client_secrets = None
with open('client_secrets.json', 'r') as f:
    client_secrets = json.load(f)

token_uri = client_secrets['installed']['token_uri']
client_id = client_secrets['installed']['client_id']
client_secret = client_secrets['installed']['client_secret']
redirect_uri = client_secrets['installed']['redirect_uris'][0]
grant_type = 'authorization_code'

data = ''
data += '%s=%s&' % ('code',          code)
data += '%s=%s&' % ('client_id',     client_id)
data += '%s=%s&' % ('client_secret', client_secret)
data += '%s=%s&' % ('redirect_uri',  redirect_uri)
data += '%s=%s' % ('grant_type',     grant_type)

request = urllib2.Request(token_uri, data)
conn = urllib2.urlopen(request)

print 'Request header items:', request.header_items()
print 'Request data:', request.get_data()
print ''
print 'HTTPs status:', conn.getcode()
print 'HTTPs url:', conn.geturl()
print 'META info:', conn.info()

response = conn.read()
conn.close()
print response
