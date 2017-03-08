
docker save serve.deploy | bzip2 | ssh -i "team11-ec2-key-pair.pem" ec2-user@ec2-52-26-164-101.us-west-2.compute.amazonaws.com 'bunzip2 | docker load'
ssh -t -i "team11-ec2-key-pair.pem" ec2-user@ec2-54-241-154-77.us-west-1.compute.amazonaws.com << 'EOF'
	docker stop $(docker ps -a -q)
	docker rm $(docker ps -a -q)
	docker run --rm -t -p 9999:9999 serve.deploy
EOF