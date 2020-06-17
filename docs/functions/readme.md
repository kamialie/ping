# Available functions

+ [getpid](#getpid)
+ [getuid](#getuid)
+ [getaddrinfo](#getaddrinfo)
+ [gettimeofday](#gettimeofday)
+ [inet\_ntop](#inet\_ntop)
+ [inet\_pton](#inet\_pton)
+ [exit](#exit)
+ [signal](#signal)
+ [alarm](#alarm)
+ [setsockopt](#setsockopt)
+ [recvmsg](#recvmsg)
+ [sendto](#sendto)
+ [socket](#socket)

## getpid

```c
#include <unistd.h>
#include <sys/types.h>

pid_t getpid(void)
```

returns parent or calling process ID, guaranteed to be unique and is useful for
constructing temporary file names; always successful, no return values is
reserved to indicate an error.

## getuid

```c
#include <unistd.h>
#include <sys/types.h>

uid_t getpid(void);
```
returns the real user ID of the calling process (who has invoked the program);
always successful, no return values is reserved to indicate an error.

## getaddrinfo

```c
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int getaddrinfo(const char *node, const char *service,
				   const struct addrinfo *hints,
				   struct addrinfo **res);
```

## gettimeofday

```c
#include <sys/time.h>

int gettimeofday(struct timeval *tv, struct timezone *tz);
```

The tv argument is a struct timeval (as specified in `<sys/time.h>`):

```c
struct timeval {
   time_t      tv_sec;     /* seconds */
   suseconds_t tv_usec;    /* microseconds */
};
```

gives the number of seconds and microseconds since Epoch.

The tz argument is a struct timezone:

```c
struct timezone {
   int tz_minuteswest;     /* minutes west of Greenwich */
   int tz_dsttime;         /* type of DST correction */
};
```

If either `tv` or `tz` is NULL, the corresponding structure is not set or
returned. The use of the timezone structure is obsolete; the tz argument
should normally be specified as NULL.

Return 0 for success and -1 for failure.

## inet\_ntop

```c
#include <arpa/inet.h>

const char *inet_ntop(int af, const void *src,
					 char *dst, socklen_t size);
```

This function converts the network address structure src in the `af`
address family into a character string.  The resulting string is copied
to the buffer pointed to by `dst`, which must be a non-null pointer.
The caller specifies the number of bytes available in this buffer
in the argument `size`.

The following address families are currently supported:
+ `AF_INET` - `src` points to a struct in\_addr (in network byte order) which
is converted to an IPv4 network address in the dotted-decimal format,
`ddd.ddd.ddd.ddd`. The buffer dst must be at least `INET_ADDRSTRLEN` bytes long.
+ `AF_INET6` - `src` points to a struct in6\_addr (in network byte order) which
is converted to a representation of this address in the most appropriate IPv6
network address format for this address. The buffer dst must be at least
`INET6_ADDRSTRLEN` bytes long.

Returns non-null `dst` on success, null indicating failure.

## inet\_pton

```c
#include <arpa/inet.h>

int inet_pton(int af, const char *src, void *dst);
```

This function converts the character string `src` into a network
address structure in the af address family, then copies the network
address structure to dst.  The `af` argument must be either `AF_INET` or
`AF_INET6`.  dst is written in network byte order.

[to be continued]

Returns 1 on success, 0 if `src` does not contain a character string
representing a valid network address in the specified address family,
-1 if `af` does not contain  valid address family.

## exit
## signal

```c
#include <signal.h>

sighandler_t signal(int signum, sighandler_t handler);
```

[to be continued]
## alarm

```c
#include <unistd.h>

unsigned int alarm(unsigned int seconds);
```

alarm() arranges for a SIGALRM signal to be delivered to the calling
process in seconds seconds. If seconds is zero, any pending alarm is canceled.
In any event any previously set alarm() is canceled.

alarm() returns the number of seconds remaining until any previously
scheduled alarm was due to be delivered, or zero if there was no
previously scheduled alarm.

## setsockopt

```c
#include <sys/socket.h>

int setsockopt(int socket, int level, int option_name,
				const void *option_value, socklen_t option_len);
```

[to be continued]

## recvmsg

```c
#include <sys/types.h>
#include <sys/socket.h>

ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);
```

[to be continued]

## sendto

```c
#include <sys/types.h>
#include <sys/socket.h>

ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
					  const struct sockaddr *dest_addr, socklen_t addrlen);
```

[to be continued]

## socket

```c
#include <sys/types.h>
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
```

[to be continued]
