/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:14:30 by dderny            #+#    #+#             */
/*   Updated: 2025/03/17 17:08:58 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_error.h"
#include "libft.h"
#include "packet.h"
#include <ft_printf.h>
#include <stdlib.h>
#include <unistd.h>

int	init_server(void)
{
	if (DEBUG)
		ft_printf("Server PID: %d\n", getpid());
	else
		ft_printf("%d\n", getpid());
	return (EXIT_SUCCESS);
}

#if SERVER

void	net_hook_connect(t_user *user)
{
	if (DEBUG)
		ft_printf("%d connected.\n", user->pid);
}

void	net_hook_loop(__attribute__((unused)) t_user *user)
{
}

void	net_hook_received(t_user *user)
{
	size_t	curlen;

	if (!user->msg)
	{
		user->len = net_read_uint32(user);
		user->msg = (char *)ft_calloc(user->len + 1, sizeof(char));
		if (!user->msg)
			return ;
	}
	curlen = ft_strlen(user->msg);
	net_read_string(user, user->msg + curlen);
	curlen = ft_strlen(user->msg);
	if (curlen >= user->len)
	{
		ft_printf("%s\n", user->msg);
		free(user->msg);
		user->msg = NULL;
	}
}

void	net_hook_sended(__attribute__((unused)) t_user *user)
{
}

#endif
