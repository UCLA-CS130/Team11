FROM ubuntu:14.04

RUN apt-get update
RUN apt-get install -y libboost-all-dev
RUN apt-get install -y make
RUN apt-get install -y g++

WORKDIR /opt/webserver
COPY . /opt/webserver

RUN make clean && make serve

EXPOSE 9999:9999
CMD ["./serve", "new_config"]