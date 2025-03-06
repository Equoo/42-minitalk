/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:52:58 by dderny            #+#    #+#             */
/*   Updated: 2025/03/06 13:24:38 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packet.h"
#include "minitalk.h"
#include "ft_error.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <libft.h>
#include <ft_printf.h>
#include <sys/types.h>
#include <unistd.h>

int	init_client(char **argv)
{
	pid_t	pid;
	t_user	*server;

	pid = ft_atoi(argv[1]);
	if (pid < 1)
		return (print_cerror("Usage", ERRINITUSAGE));
	if (ft_strlen(argv[2]) > MSG_MAX)
		return (print_cerror("Message", ERRINITLEN));
	ft_printf("Client PID: %d\n", getpid());
	server = connectto(pid);
	if (!server)
		return (print_cerror("Initialization", ERRINITPID));
	server->msg = argv[2];
	return (EXIT_SUCCESS);
}

#if CLIENT

void	net_user_connected(t_user *user)
{
	printf("Connected to %d, %s\n", user->pid, user->msg);
	if (!user->msg)
	{
		disconnect_user(user->pid, TRUE, FALSE);
		return ;
	}
	net_new_packet(user, FALSE);
	net_write_uint32(user, (uint32_t)ft_strlen(user->msg));
	user->msg = (char *)net_write_string(user, user->msg);
	net_send_packet(user);
}

void	net_hook_loop(t_user *user)
{
	if (!user || !user->msg || !*user->msg)
		return ;
	printf("Hook loop %d, %d, %s\n", user->pid, user->state, user->msg);
	if (user->state == USER_NONE)
	{
		net_new_packet(user, FALSE);
		user->msg = (char *)net_write_string(user, user->msg);
		net_send_packet(user);
	}
	else
	{
		disconnect_all(FALSE);
		exit(EXIT_SUCCESS);
	}
}

void		net_receive_packet(t_user *user, const uint8_t *data)
{
	printf("Receive packet %d, %s\n", user->pid, data);

	(void)data;
	disconnect_user(user->pid, FALSE, FALSE);
}

#endif
