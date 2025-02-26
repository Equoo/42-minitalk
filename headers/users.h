/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   users.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 11:22:19 by dderny            #+#    #+#             */
/*   Updated: 2025/02/26 15:57:03 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USERS_H
# define USERS_H

# include <ft_bool.h>
# include <signal.h>
# include <stddef.h>
# include <stdint.h>

# ifndef USERS_MAX
#  define USERS_MAX 16
# endif

typedef struct s_user
{
	pid_t		pid;
	t_bool		connected;
	uint64_t	buffer;
	uint64_t	nbr;
	uint8_t		rbytes;
	size_t		len;
	uint64_t	hash;
	char		*msg;
}		t_user;

void	disconnect_user(int pid, t_bool err, t_bool retry);
void	disconnect_all(t_bool err);
t_user	*getuser(int pid);

#endif
