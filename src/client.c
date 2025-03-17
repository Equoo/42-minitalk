/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:52:58 by dderny            #+#    #+#             */
/*   Updated: 2025/03/17 16:01:30 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_error.h"
#include "minitalk.h"
#include "packet.h"

#include <ft_printf.h>
#include <libft.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int	init_client(char **argv)
{
	pid_t	pid;
	t_user	*server;

	pid = ft_atoi(argv[1]);
	if (pid < 1)
		return (print_cerror(ERRUSAGE, ERRINITUSAGE));
	if (ft_strlen(argv[2]) > MSG_MAX)
		return (print_cerror(ERRMESS, ERRINITLEN));
	if (DEBUG)
		ft_printf("Client PID: %d\n", getpid());
	server = user_connect(pid);
	if (!server || errno)
		return (print_error(ERRUSER));
	server->msg = argv[2];
	return (EXIT_SUCCESS);
}

#if CLIENT

void	net_hook_connect(t_user *user)
{
	if (!user->msg)
	{
		user_disconnect(user->pid);
		exit(EXIT_SUCCESS);
	}
	net_new_packet(user, FALSE);
	net_write_uint32(user, (uint32_t)ft_strlen(user->msg));
	user->msg = (char *)net_write_string(user, user->msg);
	net_send_packet(user);
	if (errno == ESRCH && print_cerror(ERRUSER, ERRUSER_SERVER_TIMEOUT))
		user_kickall(TRUE, TRUE);
}

void	net_hook_loop(t_user *user)
{
	if (!user || !user->msg || !*user->msg || !user->bit
		|| user->state != USER_CONNECTED)
		return ;
	net_new_packet(user, FALSE);
	user->msg = (char *)net_write_string(user, user->msg);
	net_send_packet(user);
	if (errno == ESRCH && print_cerror(ERRUSER, ERRUSER_SERVER_TIMEOUT))
		user_kickall(TRUE, TRUE);
}

void	net_hook_received(__attribute__((unused)) t_user *user)
{
	print_cerror(ERRPACKETS, ERRPACKETS_ANORMAL);
}

void	net_hook_sended(t_user *user)
{
	if (!user->msg)
	{
		user_disconnect(user->pid);
		exit(EXIT_SUCCESS);
	}
}

#endif
