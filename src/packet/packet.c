/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:13:05 by dderny            #+#    #+#             */
/*   Updated: 2025/03/14 15:58:19 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packet.h"

#include <libft.h>
#include <ft_printf.h>
#include <ft_error.h>

t_packet	*net_new_packet(t_user *user, t_bool error)
{
	user->state = USER_RECEIVING;
	user->packet = (t_packet){0};
	user->packet.error = error;
	return (&user->packet);
}

uint8_t	net_send_error(t_user *user, const uint32_t error)
{
	t_packet	*packet;

	ft_printf("%f| Send error %d to %d\n", get_time(), error, user->pid);
	net_new_packet(user, TRUE);
	packet = &user->packet;
	user->state = USER_WRITING;
	packet->hash = error;
	*(uint32_t *)packet->data = packet->hash;
	packet->cursor = 0;
	kill(user->pid, SIGUSR1);
	return (0);
}

uint8_t	 net_send_packet(t_user *user)
{
	t_packet	*packet;

	//ft_printf("%f| Send packet\n", get_time());
	packet = &user->packet;
	user->state = USER_WRITING;
	packet->hash = hash(packet->data + sizeof(uint32_t));
	*(uint32_t *)packet->data = packet->hash;
	ft_print_memory(user->packet.data + 4, PACKET_SIZE / 8);
	packet->cursor = 0;
	kill(user->pid, SIGUSR2);
	return (0);
}
