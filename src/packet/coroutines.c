/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coroutines.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 17:58:04 by dderny            #+#    #+#             */
/*   Updated: 2025/03/12 18:23:59 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packet.h"
#include "ft_error.h"

#include <ft_printf.h>
#include <stdint.h>

void	receiving_coroutine(t_user *user)
{
	t_packet	*packet;

	//ft_printf("receiving coroutine, %d, BIT %d\n", user->state, user->bit);
	packet = &user->packet;
	//if (packet->cursor >= 1040)
		//ft_printf("Receive data %d, cursor %d\n", user->bit, packet->cursor);
	packet->data[packet->cursor / 8] <<= 1;
	packet->data[packet->cursor / 8] += user->bit;
	packet->cursor++;
	user->sbit = 1;
	if ((!packet->error && packet->cursor < PACKET_SIZE + sizeof(uint32_t) * 8)
		|| (packet->error && packet->cursor < sizeof(uint32_t) * 8))
		return ;
	packet->hash = *(uint32_t *)packet->data;
	if (!packet->error && packet->hash != hash(packet->data + sizeof(uint32_t)))
	{
		ft_printf("%x %x %x %x\n", packet->data[128], packet->data[129], packet->data[130], packet->data[131]);
		ft_printf("%f| Packet from %d failed, retry... (%x, %x) (%u %s)\n", get_time(), user->pid, packet->hash, hash(packet->data + sizeof(uint32_t)), *(uint32_t *)(packet->data + sizeof(uint32_t)), packet->data + sizeof(uint32_t) * 2);
		net_send_error(user, ERRPACKETS_DIFFER);
		return ;
	}
	//ft_printf("%f| Packet from %d: %s\n", get_time(), user->pid, packet->data + sizeof(uint32_t));
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
	//ft_printf("sending coroutine, HASH %x, CURSOR %d\n", packet->hash, packet->cursor);
	if (!packet->hash || !user->bit)
		return ;
	if ((!packet->error && packet->cursor >= PACKET_SIZE + sizeof(uint32_t) * 8)
		|| (packet->error && packet->cursor >= sizeof(uint32_t) * 8))
	{
		ft_printf("%x %x %x %x\n", packet->data[128], packet->data[129], packet->data[130], packet->data[131]);
		ft_printf("%f| Sended packet to %d: %x, len %u for %s\n", get_time(), user->pid, packet->hash, ((uint32_t *)packet->data)[1], packet->data + sizeof(uint32_t));
		user->state = USER_NONE;
		user->lastpacket = user->packet;
		user->packet = (t_packet){0};
		return ;
	}
	pos = packet->cursor / 8;
	wbytes = packet->cursor % 8;
	user->sbit = (((packet->data[pos] << wbytes) & (uint8_t)0x80) > 0);
	//if (packet->cursor >= 1040)
		//ft_printf("Sending data %d, cursor %d\n", user->sbit, packet->cursor);
	packet->cursor++;
}
