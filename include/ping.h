#ifndef PING_H
# define PING_H

# include <stdlib.h> // u_int8_t
# include <netinet/in.h> // protocol macros

/*
** global variable controlling the flow of the program
** can take the following values
*/
# define DO_NOTHING 0
# define SEND_PACKET 1
# define EXIT 2

# define DEFAULT_ICMP_DATA_SIZE 56

# define ICMP_MINIMUM_SIZE 24

/*
** in seconds
*/
# define DEFAULT_TIMEOUT 2

# define DEFAULT_TTL 37

# define SINGLE_OPTIONS "hv"
# define OPTIONS_WITH_ARGUMENTS "cptls"

# define H_FLAG 0x1
# define V_FLAG 0x2
# define T_FLAG 0x4
# define C_FLAG 0x8
# define P_FLAG 0x10
# define L_FLAG 0x20
# define S_FLAG 0x40

/*
** you must be laughing :)
*/
# define RECVMSG_ERROR 3
# define GETTIMEOFDAY_ERROR 5
# define SIGNAL_ERROR 6
# define MALLOC_ERROR 7
# define SOCKET_ERROR 9
# define SENDTO_ERROR 10
# define SETSOCKOPT_ERROR 11
# define PATTERN_ERROR 12

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
** total 8 + 56 = 64 (default)
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
}					t_msg_in;

typedef struct		s_rt_stats {
	long			min;
	long			max;
	long			sum;
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
	int 		icmp_data_size;
}					t_options;

typedef struct		s_info {
	t_options 			options;
	int					sfd_out;
	int					sfd_in;
	int 				icmp_size;
	pid_t				pid;
	char				dst_char[INET_ADDRSTRLEN];
	struct sockaddr_in	address_info;
	t_rt_stats			*rt_stats;
	t_icmp_pack			*icmp_packet;
}					t_info;

/*
** options
*/

void				options(int argv, char *args[], t_options *options);

/*
** address
*/

struct sockaddr_in	get_address(char *input);

/*
** packet
*/
t_icmp_pack			*get_icmp_packet(t_info *info);
void				update_icmp_packet(int seq, int icmp_size, t_icmp_pack *p);
int					send_packet(struct sockaddr_in *sin, t_info *info);
void				verify_received_packet(t_msg_in *msg, t_rt_stats *stats, t_info *info);

/*
** socket
*/

int					get_socket_out(t_options *opt);
int 				get_socket_in(void);

/*
** output
*/
void				print_usage(void);
void				print_execution_intro(char *input, char *dst, int icmp_data_size);
void				print_trip_stats(int ttl, double time, char *address, u_int16_t seq, int icmp_size);
void				print_execution_summary(int icmp_size, char *dst, t_rt_stats *stats);
void				print_memory(void *memory, unsigned int len);

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

/*
** exit
*/
void				exit_program(t_msg_in *msg, t_info *info);
void				exit_with_error(int code);

#endif
