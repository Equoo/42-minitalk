/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:52:58 by dderny            #+#    #+#             */
/*   Updated: 2025/03/04 17:30:28 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packet.h"
#include "minitalk.h"
#include "ft_error.h"

#include <stdlib.h>
#include <libft.h>
#include <ft_printf.h>
#include <string.h>
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
	net_new_packet(user, FALSE);
	net_write_uint32(user, ft_strlen(user->msg));
	user->msg = (char *)net_write_string(user, user->msg);
	net_send_packet(user);
}

void	net_hook_loop(t_user *user)
{

}

#endif
