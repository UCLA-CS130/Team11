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

serv.kill()