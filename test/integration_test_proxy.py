#referenced from 
#https://github.com/UCLA-CS130/TeamHello/blob/master/integration_test_proxy.py

import subprocess
import os
import time



server = subprocess.Popen(['../serve', '../new_config'])
time.sleep(0.03)

# make two request to the , one to the proxy server, one to the ucla.edu
curl_proc = subprocess.call(["curl", "-s", "localhost:9999", "-o", "out1"])
curl_proc = subprocess.call(["curl", "-s", "ucla.edu", "-o", "out2"])


#calculate the difference for the two response 
diff_proc = subprocess.Popen(["diff", "-u", "output1", "output2"], stdout=subprocess.PIPE)
diff = diff_proc.stdout.read().decode('utf-8')

server.kill()
os.remove('out1')
os.remove('out2')

if diff != '':
	print "TEST FAILED!"
	print diff
	exit(1)
else:
	print "TEST SUCCEEDED!"