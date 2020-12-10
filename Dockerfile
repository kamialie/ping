FROM debian:9

WORKDIR /app/ping

RUN apt-get update && \
	apt-get install -y make gcc tcpdump man-db valgrind netbase