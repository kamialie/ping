# ping

Recode of `ping` command line tool.

## Available functions

+ getpid
+ getuid
+ getaddrinfo
+ gettimeofday
+ inet\_ntop
+ inet\_pton
+ exit
+ signal
+ alarm
+ setsockopt
+ recvmsg
+ sendto
+ socket

Must be permorned in Linux VM (kernel > 3.14)

## Tasks

+ `-v -h` flags
+ Manage simple IPv4 hostname/address as parameter
+ manage FQDN without DNS resolution in the packet return
