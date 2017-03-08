
docker save serve.deploy | bzip2 | ssh -v -i "team11-ec2-key-pair.pem" ec2-52-26-164-101.us-west-2.compute.amazonaws.com 'bunzip2 | docker load'
ssh -v -i "team11-ec2-key-pair.pem" ec2-user@ec2-52-26-164-101.us-west-1.compute.amazonaws.com


