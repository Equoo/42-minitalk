/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 22:18:56 by dderny            #+#    #+#             */
/*   Updated: 2025/03/17 11:08:58 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_error.h"
#include "minitalk.h"
#include "packet.h"

#include <errno.h>
#include <ft_printf.h>
#include <stdlib.h>
#include <unistd.h>

/**
** Minitalk 15 000 / seconds MAC
** Minitalk 24 000 / seconds DELL
*/

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
		user->packet = user->lastpacket;
		user->packet.cursor = 0;
		net_send_packet(user);
	}
	else
	{
		user_kickall(TRUE, FALSE);
	}
}

void	net_connection_timeout(void)
{
	if (CLIENT)
	{
		print_cerror(ERRUSER, ERRUSER_TIMEOUT);
		user_kickall(TRUE, TRUE);
	}
	else
		user_kickall(FALSE, FALSE);
}
