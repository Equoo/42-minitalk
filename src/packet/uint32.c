/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uint32.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:31:16 by dderny            #+#    #+#             */
/*   Updated: 2025/03/06 13:24:55 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packet.h"

uint8_t	net_write_uint32(t_user *user, const uint32_t nbr)
{
	uint8_t		*data;
	t_packet	*packet;

	packet = &user->packet;
	if (packet->cursor + sizeof(uint32_t) * 8 > PACKET_SIZE)
		return (1);
	data = packet->data + sizeof(uint32_t) + packet->cursor / 8;
	*(uint32_t *)data = nbr;
	packet->cursor += sizeof(uint32_t) * 8;
	return (0);
}
