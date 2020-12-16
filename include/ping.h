#ifndef PING_H
# define PING_H

# include <stdlib.h> // u_int8_t
# include <netinet/in.h> // protocol macros

# define DO_NOTHING 0
# define SEND_PACKET 1
# define EXIT 2

# define DEFAULT_ICMP_DATA 56

/*
** in seconds
*/
# define DEFAULT_TIMEOUT 2

# define DEFAULT_TTL 37

# define H_FLAG 0x1
# define T_FLAG 0x2
# define C_FLAG 0x4
# define P_FLAG 0x8
# define L_FLAG 0x10

# define COUNT_OPTION_ERROR 1
# define TTL_OPTION_ERROR 2
# define PRELOAD_ERROR 3
# define RECVMSG_ERROR 4
# define GETTIMEOFDAY_ERROR 5
# define SIGNAL_ERROR 6
# define MALLOC_ERROR 7
# define SOCKET_ERROR 8
# define SENDTO_ERROR 9
# define SETSOCKOPT_ERROR 10
# define PATTERN_ERROR 11

/*
** 8 bytes
** type - message type, 0 - echo reply, 8 - echo request
** code - used for error code
** chksum - checksum of header plus data
** id - identifier used to match requests/replies
** seq - used to identify the sequence if more than one is sent
*/
typedef struct		s_icmp_hdr {
	u_int8_t	type;
	u_int8_t	code;
	u_int16_t	chksum;
	u_int16_t	id;
	u_int16_t	seq;
}					t_icmp_hdr;

/*
** total 8 + 56 = 64
*/
typedef struct		s_icmp_pack {
	t_icmp_hdr		header;
	struct timeval	tv;
	void 			*pad;
}					t_icmp_pack;

/*
** 20 bytes
*/
typedef struct		s_ip_hdr {
	u_int8_t	iph_ihl:4;
	u_int8_t	ip_ver:4;
	u_int8_t	iph_tos;
	u_int16_t	iph_len;
	u_int16_t	iph_ident;
	u_int16_t	iph_flags:3;
	u_int16_t	iph_offset:13;
	u_int8_t	iph_ttl;
	u_int8_t	iph_protocol;
	u_int16_t	iph_chksum;
	u_int32_t	iph_source;
	u_int32_t	iph_dest;
}					t_ip_hdr;

typedef struct		s_msg_in {
	struct sockaddr_in	rec_addr;
	struct iovec		io;
	struct msghdr		msghdr;
	char				buf[512];
}					t_msg_in;

typedef struct		s_rt_stats {
	long			min;
	long			max;
	long			sum;
	long			sum2;
	u_int16_t		pkg_sent;
	u_int16_t		pkg_received;
	u_int16_t		errors;
	struct timeval	tv_start;
}					t_rt_stats;

typedef struct		s_options
{
	int			options;
	int			ttl;
	int			count;
	int			preload;
	long int	pattern;
	int 		patternlen;
}					t_options;

typedef struct		s_info {
	t_options 			options;
	int					sfd_out;
	int					sfd_in;
	pid_t				pid;
	char				dst_char[INET_ADDRSTRLEN];
	struct sockaddr_in	address_info;
	// TODO decide if need to be malloced
	t_rt_stats			*rt_stats;
	t_icmp_pack			*icmp_packet;
}					t_info;

int					options(int argv, char *args[], t_options *options);
struct sockaddr_in	get_address(char *input);
//int					get_socket(void);
t_icmp_pack			*get_icmp_packet(t_info *info);
void				update_icmp_packet(int seq, t_icmp_pack *p);
int				send_packet(int sfd, t_icmp_pack *packet,
										struct sockaddr_in *sin);
void				verify_received_packet(int pid, t_rt_stats *stats, t_msg_in *msg);

/*
** socket
*/

int					get_socket_out(t_options *opt);
int 				get_socket_in(void);

/*
** output
*/
void				print_usage(void);
void				print_execution_intro(char *input, char *dst);
void				print_trip_stats(int ttl, double time, char *address, t_icmp_pack *icmp_in);
void				print_trip_error(t_icmp_pack *icmp_in, char *address);
void				print_execution_summary(char *dst, t_rt_stats *stats);
void				print_memory(void *memory, unsigned int len);
void				exit_with_error(int code);

/*
** stats
*/
long				get_trip_time(struct timeval *tv_begin);
double				update_rt_stats(struct timeval *tv_in, t_rt_stats *stats);

/*
** utils
*/
u_int16_t			compute_checksum(u_int16_t *addr, int count);
u_int16_t			ft_htons(u_int16_t x);
u_int16_t			ft_ntohs(u_int16_t x);
u_int64_t			ft_htonll(u_int64_t x);

void				exit_program(t_info *info);

#endif
