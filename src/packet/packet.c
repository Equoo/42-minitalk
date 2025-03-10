/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:13:05 by dderny            #+#    #+#             */
/*   Updated: 2025/03/10 14:43:35 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packet.h"
#include <stdio.h>

t_packet	*net_new_packet(t_user *user, t_bool error)
{
	user->state = USER_RECEIVING;
	user->packet = (t_packet){0};
	user->packet.error = error;
	return (&user->packet);
}

uint8_t	net_send_error(t_user *user, const uint32_t error)
{
	net_new_packet(user, TRUE);

	printf("Send error %d to %d, %d\n", error, user->pid, user->packet.error);

	net_write_uint32(user, error);
	net_send_packet(user);
	return (0);
}

uint8_t	 net_send_packet(t_user *user)
{
	t_packet	*packet;

	packet = &user->packet;
	if (!packet->hash)
	{
		user->state = USER_WRITING;
		packet->hash = hash(packet->data + sizeof(uint32_t));
		*(uint32_t *)packet->data = packet->hash;
		packet->cursor = 0;
		kill(user->pid, SIGUSR2);
	}
	return (0);
}
