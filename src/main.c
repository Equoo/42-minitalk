/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 22:18:56 by dderny            #+#    #+#             */
/*   Updated: 2025/03/05 15:36:32 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"
#include "ft_error.h"
#include "packet.h"

#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <ft_printf.h>

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
	if (error == ERRPACKETS_DIFFER)
	{
		net_new_packet(user, FALSE);
		user->packet = user->lastpacket;
		net_send_packet(user);
	}
	else if (error == ERRPACKETS_TIMEOUT)
		ft_printf("Packet from %d timed out, retry...\n", user->pid);
}

void	net_connection_timeout(void)
{
	ft_printf("Connection timeout\n");
	if (CLIENT)
		disconnect_all(TRUE);
	else
		disconnect_all(FALSE);
}
