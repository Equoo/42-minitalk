/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uint32.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:31:16 by dderny            #+#    #+#             */
/*   Updated: 2025/03/12 18:19:28 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packet.h"
#include <stdint.h>

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

uint32_t net_read_uint32(t_user *user)
{
	uint8_t		*data;
	t_packet	*packet;
	uint32_t	nbr;

	packet = &user->packet;
	data = packet->data + sizeof(uint32_t) + packet->cursor / 8;
	nbr = *(uint32_t *)data;
	packet->cursor += sizeof(uint32_t) * 8;
	return (nbr);
}
