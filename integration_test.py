import sys, os, subprocess
import requests

class ExpectedResponse: 
  body = ""
  content_type = ""
  status = ""

  def __init__(self, body, content_type, status):
    self.body = body
    self.content_type = content_type
    self.status = status

def send_request(req, expected):
  print 'Performing the following request: ', req
  try:
    r = session.get(req)
  except requests.exceptions.RequestException:
    serv.kill(); 
    print 'Connection refused'
    sys.exit(1)

  print 'Status code is:',r.status_code
  print 'Content-type is:',r.headers['content-type']

  # Checker headers
  if r.status_code != expected.status:
    print 'ERROR: Request to website failed'
    sys.exit(1) 

  # Check content-type
  if r.headers['content-type'] != expected.content_type:
    print 'ERROR: Incorrect content type'
    sys.exit(1) 

  # Check response
  if expected.body in r.text:
    print 'Expected response received!'
  else:  
    print 'ERROR: Incorrect response: '
    print r.text
    sys.exit(1)


DEVNULL = open(os.devnull, 'wb')

print 'Running integration test on webserver...'

print 'Building the binary...'
if subprocess.call(["sudo","make"]) != 0:
  print 'ERROR: Build failed'
  sys.exit(1)

print 'Running webserver...'
serv = subprocess.Popen(["./serve", "demo_config"])

print 'Sending requests to server...'
# Proxy issue fix:
session = requests.Session()
session.trust_env = False

echo = ExpectedResponse("GET /echo HTTP/1.1\r\nHost: localhost:9999", "text/plain", 200)
static = ExpectedResponse("<html>\n\t<h1> This is an example html file </h1>", "text/html", 200)
bad = ExpectedResponse("<html><h1> 404 Not Found </h1></html", "text/html", 404)

send_request('http://localhost:9999/echo', echo)
send_request('http://localhost:9999/static1/hello.html', static)
send_request('http://localhost:9999/static1/missing.jpg', bad)

print 'Terminating webserver...'
serv.kill(); 
