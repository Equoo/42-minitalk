/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coroutines.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 17:58:04 by dderny            #+#    #+#             */
/*   Updated: 2025/03/18 11:39:51 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "packet.h"
#include "ft_error.h"

#include <ft_printf.h>
#include <errno.h>
#include <stdint.h>

static void	reception_coroutine(t_user *user, t_packet *packet)
{
	packet->hash = *(uint32_t *)packet->data;
	if (DEBUG)
		ft_print_memory(packet->data, PACKET_SIZE / 8 + 4);
	if (!packet->error && packet->hash != hash(packet->data + sizeof(uint32_t)))
	{
		user->sbit = -1;
		net_send_error(user, ERRPACKETS_DIFFER);
		if (errno)
			user_kickall(FALSE, FALSE);
		return ;
	}
	user->state = USER_CONNECTED;
	if (packet->error)
		net_receive_error(user, packet->hash);
	else
	{
		packet->cursor = 0;
		net_hook_received(user);
		user->lastpacket = *packet;
		user->packet = (t_packet){0};
	}
}

void	receiving_coroutine(t_user *user)
{
	t_packet	*packet;

	packet = &user->packet;
	packet->data[packet->cursor / 8] <<= 1;
	packet->data[packet->cursor / 8] += user->bit;
	packet->cursor++;
	user->sbit = 1;
	if ((!packet->error && packet->cursor < PACKET_SIZE + sizeof(uint32_t) * 8)
		|| (packet->error && packet->cursor < sizeof(uint32_t) * 8))
		return ;
	reception_coroutine(user, packet);
}

void	sending_coroutine(t_user *user)
{
	t_packet	*packet;
	uint32_t	wbytes;
	uint32_t	pos;

	packet = &user->packet;
	if (!packet->hash)
		return ;
	if ((!packet->error && packet->cursor >= PACKET_SIZE + sizeof(uint32_t) * 8)
		|| (packet->error && packet->cursor >= sizeof(uint32_t) * 8))
	{
		if (DEBUG)
			ft_print_memory(packet->data, PACKET_SIZE / 8 + 4);
		user->state = USER_CONNECTED;
		user->lastpacket = user->packet;
		user->packet = (t_packet){0};
		net_hook_sended(user);
		if (errno && print_error("Sended hook"))
			user_kickall(TRUE, TRUE);
		return ;
	}
	pos = packet->cursor / 8;
	wbytes = packet->cursor % 8;
	user->sbit = (((packet->data[pos] << wbytes) & (uint8_t)0x80) > 0);
	packet->cursor++;
}
