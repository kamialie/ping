FROM debian:9

RUN apt-get update && \
	apt-get install -y make gcc