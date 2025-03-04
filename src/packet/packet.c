/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:13:05 by dderny            #+#    #+#             */
/*   Updated: 2025/03/04 15:31:46 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packet.h"

t_packet	*net_new_packet(t_user *user, t_bool error)
{
	user->state = USER_RECEIVING;
	user->packet = (t_packet){0};
	user->packet.error = error;
	return (&user->packet);
}

uint8_t	net_send_packet(t_user *user)
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
