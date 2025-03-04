/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 17:28:18 by dderny            #+#    #+#             */
/*   Updated: 2025/03/03 19:06:39 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PACKET_H
# define PACKET_H

#include "ft_bool.h"
#include <stdint.h>

# ifndef PACKET_SIZE
#  define PACKET_SIZE 256
# endif

typedef  struct	s_packet
{
	t_bool		error;
	uint32_t	hash;
	uint8_t		data[PACKET_SIZE + sizeof(uint32_t)];
	uint32_t	cursor;
}				t_packet;

int			net_send_error(int pid, int error);
int			net_send_packet(int pid, t_packet *packet);

void		net_receive_packet(int pid, const uint8_t *data);

uint64_t	net_write_nbr(t_packet *packet, uint64_t nbr, uint8_t size);
uint64_t	net_read_nbr(t_packet *packet, uint64_t nbr, uint8_t size);

#endif