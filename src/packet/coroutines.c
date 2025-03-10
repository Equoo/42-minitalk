/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coroutines.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 17:58:04 by dderny            #+#    #+#             */
/*   Updated: 2025/03/10 18:41:18 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packet.h"
#include "ft_error.h"

#include <ft_printf.h>

void	receiving_coroutine(t_user *user)
{
	t_packet	*packet;

	packet = &user->packet;
	packet->data[packet->cursor / 8] <<= 1;
	packet->data[packet->cursor / 8] += user->bit;
	packet->cursor++;
	user->sbit = 1;
	if ((!packet->error && packet->cursor < PACKET_SIZE + sizeof(uint32_t) * 8 - 1)
		|| (packet->error && packet->cursor < sizeof(uint32_t) * 8 - 1))
		return ;
	packet->hash = *(uint32_t *)packet->data;
	if (!packet->error && packet->hash != hash(packet->data + sizeof(uint32_t)))
	{
		ft_printf("Packet from %d failed, retry... (%x, %x) (%u %s)\n", user->pid, packet->hash, hash(packet->data + sizeof(uint32_t)), *(uint32_t *)(packet->data + sizeof(uint32_t)), packet->data + sizeof(uint32_t) * 2);
		net_send_error(user, ERRPACKETS_DIFFER);
		return ;
	}
	ft_printf("Packet from %d: %s\n", user->pid, packet->data + sizeof(uint32_t));
	user->state = USER_CONNECTED;
	if (packet->error)
		net_receive_error(user, packet->hash);
	else
	{
		net_receive_packet(user, packet->data + sizeof(uint32_t));
		user->lastpacket = *packet;
	}
	user->packet = (t_packet){0};
}

void	sending_coroutine(t_user *user)
{
	t_packet	*packet;
	uint32_t	wbytes;
	uint32_t	pos;

	packet = &user->packet;
	if (!packet->hash || !user->bit)
		return ;
	if ((!packet->error && packet->cursor >= PACKET_SIZE + sizeof(uint32_t) * 8)
		|| (packet->error && packet->cursor >= sizeof(uint32_t) * 8))
	{
		ft_printf("Sended packet to %d: %x, len %u for %s\n", user->pid, packet->hash, ((uint32_t *)packet->data)[1], packet->data + sizeof(uint32_t));
		user->state = USER_NONE;
		user->lastpacket = user->packet;
		user->packet = (t_packet){0};
		return ;
	}
	pos = packet->cursor / 8;
	wbytes = packet->cursor % 8;
	user->sbit = (packet->data[pos] << wbytes) & (uint8_t)0x80;
	packet->cursor++;
}
