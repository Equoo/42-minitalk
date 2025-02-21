/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 15:22:25 by dderny            #+#    #+#             */
/*   Updated: 2025/02/21 02:49:36 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H
# include <ft_bool.h>
# include <sys/types.h>

# ifndef CLIENT
#  define CLIENT 0
# endif

# ifndef SERVER
#  define SERVER 0
# endif

# if CLIENT == 1 && SERVER == 1
#  error "CLIENT and SERVER can't be defined at the same time"
# endif

# ifndef USERS_MAX
#  define USERS_MAX 32
# endif

# ifndef MSG_MAX
#  define MSG_MAX __LONG_MAX__
# endif

/* 
** Timeout time in seconds
*/
# ifndef TIMEOUT_TIME
#  define TIMEOUT_TIME 2
# endif

typedef struct s_user {
	int			pid;
	t_bool		connected;
	time_t		last_seen;
	size_t		nbr;
	int			rbytes;
	size_t		len;
	char		*msg;
}		t_user;

#endif