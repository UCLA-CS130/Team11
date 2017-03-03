import sys, os, subprocess
import requests
import time

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
  except requests.exceptions.ConnectionError:
    time.sleep(1)
    r = session.get(req)
  except requests.exceptions.RequestException as e:
    serv.kill()
    print e
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
serv = subprocess.Popen(["./serve", "new_config"])

# Start of Multithreading Testing

# Spawn shell process as hanging http request
telnet_request = "telnet localhost 9999"
telnet_request_process = subprocess.Popen(telnet_request, stdout=subprocess.PIPE, shell=True)

echo_request = "curl -i localhost:9999/echo"
echo_request_process = subprocess.Popen(echo_request, stdout=subprocess.PIPE, shell=True)
expected_response_first = """HTTP/1.1 200 OK\r
Content-Type: text/plain\r
\r
GET /echo HTTP/1.1\r
User-Agent: curl/"""

# splitting in half, because user-agent 'curl' is dependent on machine

expected_response_second = """Host: localhost:9999\r
Accept: */*\r\n\r\n"""

echo_response = echo_request_process.stdout.read().decode('utf-8')

print "Echo response:"
print echo_response
print "Expected response:"
print expected_response_first.decode('utf-8')
print expected_response_second.decode('utf-8')

if expected_response_first in echo_response and expected_response_second in echo_response:
  print "SUCCESS: Echo handler replied with the correct response. Multithreading works."
else:
  print "ERROR: Echo handler replied with the wrong response."
  exit(1)

# End of Multithreading Testing

print 'Sending requests to server...'
# Proxy issue fix:
session = requests.Session()
session.trust_env = False

echo = ExpectedResponse("GET /echo HTTP/1.1\r\nHost: localhost:9999", "text/plain", 200)
echo_test = ExpectedResponse("GET /echo/test HTTP/1.1\r\nHost: localhost:9999", "text/plain", 200)
static = ExpectedResponse("<html>", "text/html", 200)
bad = ExpectedResponse("<html><body><h1>404 Not Found</h1></body></html>", "text/html", 404)


send_request('http://localhost:9999/echo', echo)
# Test longest prefix mapping: 
send_request('http://localhost:9999/echo/test', echo_test)
send_request('http://localhost:9999/static1/hello.html', static)
send_request('http://localhost:9999/static1/missing', bad)

print 'Terminating webserver...'
serv.kill(); 
