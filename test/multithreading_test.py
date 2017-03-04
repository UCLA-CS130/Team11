import sys, os, subprocess
import requests
import time
import telnetlib

DEVNULL = open(os.devnull, 'wb')

print 'Running integration test on webserver...'

print 'Building the binary...'
if subprocess.call(["sudo","make"]) != 0:
  print 'ERROR: Build failed'
  sys.exit(1)

print 'Running webserver with multithreading...'
serv = subprocess.Popen(["./serve", "new_config"])

num_threads = int(sys.argv[1])
print('Thread number : '+ str(num_threads))

expected_response_first = """HTTP/1.1 200 OK\r
Content-Type: text/plain\r
\r
GET /echo HTTP/1.1\r
User-Agent: telnet"""

expected_response_second = """Host: localhost:9999\r
Accept: */*\r\n\r\n"""

for request_index in range(num_threads):
	echo_request = "curl -i localhost:9999/echo"
	echo_request_process = subprocess.Popen(echo_request, stdout=subprocess.PIPE, shell=True)
	echo_response = echo_request_process.stdout.read().decode('utf-8')
	
	print("Echo response: " + str(echo_response))

serv.kill()