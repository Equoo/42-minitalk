/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   users.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 11:21:54 by dderny            #+#    #+#             */
/*   Updated: 2025/02/25 19:27:03 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "users.h"
#include <stdint.h>
#include <stdlib.h>

t_user	users[USERS_MAX] = {0};

void	disconnect_user(pid_t pid, t_bool err, t_bool retry)
{
	int	i;

	i = 0;
	while (i < USERS_MAX && users[i].pid != pid)
		i++;
	if (i == USERS_MAX)
		return ;
	if (users[i].msg && users[i].len)
		free(users[i].msg);
	users[i] = (t_user){0};
	if (err)
	{
		kill(pid, SIGUSR1);
		if (retry)
			kill(pid, SIGUSR2);
		else
			kill(pid, SIGUSR1);
	}
	else
		kill(pid, SIGUSR2);
}

void	disconnect_all(t_bool err)
{
	int	i;

	i = 0;
	while (i < USERS_MAX)
	{
		if (users[i].connected)
			disconnect_user(users[i].pid, err, FALSE);
		i++;
	}
}

t_user	*getuser(int pid)
{
	int	i;

	if (pid < 1)
		return (NULL);
	i = 0;
	while (i < USERS_MAX && users[i].pid != pid)
		i++;
	if (i == USERS_MAX)
	{
		i = 0;
		while (users[i].connected)
			i++;
		if (i == USERS_MAX)
			return (NULL);
		users[i] = (t_user){};
		users[i].pid = pid;
	}
	return (&users[i]);
}