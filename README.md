# ping

Recode of `ping` command line tool.

## Run

Woks in Linux kernel > 3.14

+ `docker-compose up -d`
+ `docker exec -ti ping_box bash`
+ `make`
+ `ping google.com`

## Features

+ `-l preload` - send `preload` number of packets without waiting for response
+ `-c count` - stop sending packets after `count` times
+ `-s size` - specify the size of ICMP data section
+ `-p pattern` - fill in the remaining data section with `pattern`
+ `-t ttl` - change the default ttl field of IP packet

## About

[Subject and description of the project](https://www.notion.so/ping-e1868da0cf884031a0ccd740ab89d4f7)