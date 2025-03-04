/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 17:28:18 by dderny            #+#    #+#             */
/*   Updated: 2025/03/04 16:42:59 by dderny           ###   ########.fr       */
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

# ifndef PACKET_SIZE
#  define PACKET_SIZE 256
# endif

typedef enum e_user_state {
	USER_NONE,
	USER_RECEIVING,
	USER_WRITING
}	t_user_state;

typedef  struct	s_packet
{
	t_bool		error;
	uint32_t	hash;
	uint8_t		data[PACKET_SIZE + sizeof(uint32_t)];
	uint32_t	cursor;
}				t_packet;

typedef struct s_user
{
	pid_t			pid;
	t_bool			connected;
	t_user_state	state;
	char			*msg;
	size_t			len;
	t_packet		packet;
}		t_user;

uint32_t	hash(uint8_t *str);

void		disconnect_user(int pid, t_bool err, t_bool retry);
void		disconnect_all(t_bool err);
t_user		*connectto(pid_t pid);
t_user		*getuser(int pid);

uint8_t		net_init(void);
void		net_loop(void);

t_packet	*net_new_packet(t_user *user, t_bool error);
const char	*net_write_string(t_user *user, const char *str);
uint8_t		net_write_uint32(t_user *user, const uint32_t nbr);
const char	*net_read_string(t_user *user);
uint32_t	net_read_uint32(t_user *user);

int			net_send_error(t_user *user, int error);
uint8_t		net_send_packet(t_user *user);

void		net_hook_loop(t_user *user);
void		net_user_connected(t_user *user);
void		net_receive_packet(t_user *user, const uint8_t *data);
void		net_connection_timeout(void);

#endif
