FROM ubuntu:14.04

RUN apt-get update
RUN apt-get -y upgrade 
RUN apt-get install -y build-essential 
RUN apt-get install -y software-properties-common 
RUN apt-get install -y libboost-all-dev
RUN apt-get install -y make
RUN apt-get install -y g++
RUN apt-get install -y libmysqlclient-dev libmysqlcppconn-dev


WORKDIR /opt/web-serve
COPY . /opt/web-serve

RUN make clean && make

CMD ["tar", "-cf", "-", "serve"]

