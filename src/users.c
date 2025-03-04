/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   users.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:38:41 by dderny            #+#    #+#             */
/*   Updated: 2025/03/03 15:17:15 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "users.h"
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

t_user	g_users[USERS_MAX] = {0};

void	disconnect_user(pid_t pid, t_bool err, t_bool retry)
{
	int	i;

	i = 0;
	while (i < USERS_MAX && g_users[i].pid != pid)
		i++;
	if (i == USERS_MAX)
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

void	disconnect_all(t_bool err)
{
	int	i;

	i = 0;
	while (i < USERS_MAX)
	{
		if (g_users[i].connected)
			disconnect_user(g_users[i].pid, err, FALSE);
		i++;
	}
	if (err)
		exit(EXIT_FAILURE);
	else
		exit(EXIT_SUCCESS);
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
		while (g_users[i].connected)
			i++;
		if (i == USERS_MAX)
			return (NULL);
		g_users[i] = (t_user){};
		g_users[i].pid = pid;
	}
	return (&g_users[i]);
}
