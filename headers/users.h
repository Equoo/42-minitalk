/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   users.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 11:22:19 by dderny            #+#    #+#             */
/*   Updated: 2025/03/03 19:03:34 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USERS_H
# define USERS_H

#include "packet.h"
# include <ft_bool.h>
# include <signal.h>
# include <stddef.h>
# include <stdint.h>

# ifndef USERS_MAX
#  define USERS_MAX 16
# endif

typedef enum e_user_state {
	USER_RECEIVING,
	USER_WRITING
}	t_user_state;

typedef struct s_user
{
	pid_t			pid;
	t_bool			connected;
	t_user_state	state;
	char			*msg;	
	t_packet		packet;
}		t_user;

void	disconnect_user(int pid, t_bool err, t_bool retry);
void	disconnect_all(t_bool err);
t_user	*getuser(int pid);

#endif
