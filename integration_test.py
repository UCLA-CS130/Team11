import sys, os, subprocess
import requests

DEVNULL = open(os.devnull, 'wb')

print 'Running integration test on webserver...'

print 'Building the binary...'
if subprocess.call(["sudo","make"]) != 0:
  print 'ERROR: Build failed'
  sys.exit(1)

print 'Running webserver...'
serv = subprocess.Popen(["./serve", "demo_config"], stdout=DEVNULL)

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
expected_resp = 'GET / HTTP/1.1\r\nHost: localhost:9999\r\nAccept-Encoding: gzip, deflate, compress\r\nAccept: */*\r\nUser-Agent: python-requests/2.2.1 CPython/2.7.6 Linux/4.2.0-27-generic\r\n\r\n'
if r.text != expected_resp:
  print 'ERROR: Incorrect response'
  sys.exit(1) 

print 'Terminating webserver...'
serv.kill(); 
