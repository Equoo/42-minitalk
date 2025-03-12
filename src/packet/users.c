/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   users.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:38:41 by dderny            #+#    #+#             */
/*   Updated: 2025/03/12 13:47:10 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_error.h"
#include "packet.h"

#include <ft_printf.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

t_user	g_users[USERS_MAX] = {0};

t_user	*user_connect(pid_t pid)
{
	t_user	*user;

	user = getuser(pid);
	if (!user)
	{
		print_cerror(ERRUSER, ERRUSER_MAX);
		return (NULL);
	}
	if (user->state >= USER_CONNECTED)
	{
		ft_printf("User %d already connected\n", pid);
		return (NULL);
	}
	user->state = USER_CONNECTING;
	kill(pid, SIGUSR2);
	return (user);
}

void	user_disconnect(pid_t pid, t_bool err, t_bool retry)
{
	int	i;

	i = 0;
	while (i < USERS_MAX && g_users[i].pid != pid)
		i++;
	if (i == USERS_MAX)
		return ;
	if (g_users[i].state < USER_CONNECTED || g_users[i].pending)
		return ;
	if (g_users[i].msg && g_users[i].len)
		free(g_users[i].msg);
	g_users[i] = (t_user){0};
	if (err)
	{
		kill(pid, SIGUSR1);
		usleep(2500);
		if (retry)
			kill(pid, SIGUSR2);
		else
			kill(pid, SIGUSR1);
	}
	else
	{
		kill(pid, SIGUSR2);
		usleep(2500);
		kill(pid, SIGUSR2);
	}
}

void	user_kickall(t_bool err)
{
	int	i;

	i = 0;
	while (i < USERS_MAX)
	{
		if (g_users[i].state >= USER_CONNECTED)
			user_disconnect(g_users[i].pid, err, FALSE);
		i++;
	}
	if (err)
		exit(EXIT_FAILURE);
	//else
		//exit(EXIT_SUCCESS);
}

t_user	*getuser(int pid)
{
	int	i;

	if (pid < 1)
		return (NULL);
	i = 0;
	while (i < USERS_MAX && g_users[i].pid != pid)
		i++;
	if (i == USERS_MAX)
	{
		i = 0;
		while (g_users[i].state >= USER_CONNECTED)
			i++;
		if (i == USERS_MAX)
			return (NULL);
		g_users[i] = (t_user){0};
		g_users[i].pid = pid;
		g_users[i].bit = -1;
		g_users[i].sbit = -1;
	}
	return (&g_users[i]);
}


void	foruser(void (f)(t_user *user))
{
	int	i;

	i = 0;
	while (i < USERS_MAX)
	{
		if (g_users[i].state >= USER_CONNECTED)
			f(&g_users[i]);
		i++;
	}
}
