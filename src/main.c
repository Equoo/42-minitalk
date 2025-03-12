/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 22:18:56 by dderny            #+#    #+#             */
/*   Updated: 2025/03/12 16:54:13 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_error.h"
#include "minitalk.h"
#include "packet.h"
#include <errno.h>
#include <ft_printf.h>
#include <stdlib.h>
#include <unistd.h>

int	main(int argc, char **argv)
{
	if (CLIENT && argc != 3)
		return (print_cerror("Usage", ERRINITUSAGE));
	net_init();
	if (CLIENT)
		init_client(argv);
	else if (SERVER)
		init_server();
	if (errno && print_error(ERRINIT))
		return (EXIT_FAILURE);
	net_loop();
	return (EXIT_SUCCESS);
}

void	net_receive_error(t_user *user, const uint32_t error)
{
	ft_printf("%f| receive error: %d\n", get_time(), error);
	if (error == ERRPACKETS_DIFFER)
	{
		net_new_packet(user, FALSE);
		user->packet = user->lastpacket;
		net_send_packet(user);
	}
	else if (error == ERRPACKETS_TIMEOUT)
		ft_printf("%|f Packet from %d timed out, retry...\n", get_time(), user->pid);
}

void	net_connection_timeout(void)
{
	ft_printf("%f| Connection timeout\n", get_time());
	if (CLIENT)
		user_kickall(TRUE);
	else
		user_kickall(FALSE);
}
