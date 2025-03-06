/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:14:30 by dderny            #+#    #+#             */
/*   Updated: 2025/03/06 11:35:25 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <ft_printf.h>
#include <unistd.h>

#include "libft.h"
#include "packet.h"
#include "ft_error.h"

int	init_server(void)
{
	ft_printf("Server PID: %d\n", getpid());
	return (EXIT_SUCCESS);
}

#if SERVER

void	net_user_connected(t_user *user)
{
	(void)user;
}

void	net_hook_loop(t_user *user)
{
	(void)user;
}

void		net_receive_packet(t_user *user, const uint8_t *data)
{
	char	*str;
	size_t 	curlen;

	if (!user->msg)
	{
		user->len = *(uint32_t *)data;
		user->msg = (char *)malloc(sizeof(char) * (user->len + 1));
		user->msg[user->len] = 0;
		str = (char *)data + sizeof(uint32_t);
	}
	else
		str = (char *)data;
	curlen = ft_strlen(user->msg);
	if (curlen + ft_strlen(str) <= user->len)
		ft_memcpy(user->msg + curlen, str, ft_strlen(str));
	if (curlen == user->len)
	{
		ft_printf("Message from %d: %s\n", user->pid, user->msg);
		free(user->msg);
		user->msg = NULL;
		disconnect_user(user->pid, FALSE, FALSE);
	}
}

#endif

