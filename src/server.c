/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:14:30 by dderny            #+#    #+#             */
/*   Updated: 2025/03/12 11:32:40 by dderny           ###   ########.fr       */
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
	ft_printf("Server PID: %d\n", getpid());
	return (EXIT_SUCCESS);
}

#if SERVER

void	net_user_connected(t_user *user)
{
	(void)user;
	ft_printf("%f| User connected, %d\n", get_time(), user->pid);
}

void	net_hook_loop(t_user *user)
{
	(void)user;
}

void	net_receive_packet(t_user *user, const uint8_t *data)
{
	char	*str;
	size_t	strlen;
	size_t	curlen;

	if (!user->msg)
	{
		user->len = *(uint32_t *)data;
		user->msg = (char *)ft_calloc(user->len + 1, sizeof(char));
		str = (char *)data + sizeof(uint32_t);
	}
	else
		str = (char *)data;
	strlen = ft_strlen(str);
	curlen = ft_strlen(user->msg);
	if (curlen + strlen <= user->len)
		ft_memcpy(user->msg + curlen, str, ft_strlen(str));
	if (curlen + strlen == user->len)
	{
		ft_printf("%f| Message from %d: %s\n", get_time(), user->pid, user->msg);
		free(user->msg);
		user->msg = NULL;
		user_disconnect(user->pid, FALSE, FALSE);
	}
}

#endif
