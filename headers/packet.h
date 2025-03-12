/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 17:28:18 by dderny            #+#    #+#             */
/*   Updated: 2025/03/12 13:46:11 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PACKET_H
# define PACKET_H

# include <ft_bool.h>
# include <signal.h>
# include <stdint.h>

# ifndef USERS_MAX
#  define USERS_MAX 16
# endif

/*
** 128 chars = 1024 bits
*/
# ifndef PACKET_SIZE
#  define PACKET_SIZE 1024
# endif

typedef enum e_user_state {
	USER_NONE,
	USER_CONNECTING,
	USER_CONNECTION_PENDING,
	USER_CONNECTED,
	USER_RECEIVING,
	USER_WRITING,
}	t_user_state;

typedef  struct	s_packet
{
	t_bool		error;
	uint32_t	hash;
	uint8_t		data[PACKET_SIZE / 8 + sizeof(uint32_t)];
	uint32_t	cursor;
}				t_packet;

typedef struct s_user
{
	pid_t			pid;
	int8_t			bit;
	int8_t			sbit;
	t_bool			pending;
	t_user_state	state;
	t_packet		lastpacket;
	t_packet		packet;

	char			*msg;
	size_t			len;
}		t_user;

/*
** USERS BEHAVIORS
*/
void		user_disconnect(int pid, t_bool err, t_bool retry);
void		user_kickall(t_bool err);
t_user		*user_connect(pid_t pid);
t_user		*getuser(int pid);
void		foruser(void (f)(t_user *user));

/*
** INTERNAL
*/
uint8_t		net_init(void);
void		net_loop(void);
uint32_t	hash(uint8_t *str);
void		receiving_coroutine(t_user *user);
void		sending_coroutine(t_user *user);

/*
** PACKETS BEHAVIORS
*/
t_packet	*net_new_packet(t_user *user, t_bool error);

const char	*net_write_string(t_user *user, const char *str);
uint8_t		net_write_uint32(t_user *user, const uint32_t nbr);

const char	*net_read_string(t_user *user);
uint32_t	net_read_uint32(t_user *user);

uint8_t		net_send_error(t_user *user, const uint32_t error);
uint8_t		net_send_packet(t_user *user);

/*
** HOOKS
*/
void		net_hook_loop(t_user *user);
void		net_user_connected(t_user *user);
void		net_receive_error(t_user *user, const uint32_t error);
void		net_receive_packet(t_user *user, const uint8_t *data);
void		net_connection_timeout(void);

#endif
