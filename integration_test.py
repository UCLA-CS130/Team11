import sys, os, subprocess
import requests

DEVNULL = open(os.devnull, 'wb')

print 'Running integration test on webserver...'

print 'Building the binary...'
if subprocess.call(["sudo","make"]) != 0:
  print 'ERROR: Build failed'
  sys.exit(1)

print 'Running webserver...'
serv = subprocess.Popen(["./serve", "demo_config"])

print 'Sending request to webserver...'
# Fix due to proxy issues
session = requests.Session()
session.trust_env = False

try:
  r = session.get("http://localhost:9999")
except requests.exceptions.RequestException:
  serv.kill(); 
  print 'Connection refused'
  sys.exit(1)

print 'Status code is:',r.status_code
print 'Content-type is:',r.headers['content-type']

# Checker headers
if r.status_code != 200:
  print 'ERROR: Request to website failed'
  sys.exit(1) 

# Check content-type
if r.headers['content-type'] != "text/plain":
  print 'ERROR: Incorrect content type'
  sys.exit(1) 

# Check response
# We are checking using a substring as the user-agent differs between our vm and travis' vm
expected_resp = 'GET / HTTP/1.1\r\nHost: localhost:9999\r\nAccept-Encoding: gzip, deflate, compress\r\nAccept: */*\r\n'
if expected_resp in r.text:
  print 'Expected response received!'
else:  
  print 'ERROR: Incorrect response'
  sys.exit(1) 

print 'Terminating webserver...'
serv.kill(); 
