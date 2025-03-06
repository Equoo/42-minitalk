/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:30:47 by dderny            #+#    #+#             */
/*   Updated: 2025/03/06 13:31:32 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packet.h"

#include <libft.h>
#include <stdint.h>

const char	*net_write_string(t_user *user, const char *str)
{
	const size_t	size_left = (PACKET_SIZE - user->packet.cursor + sizeof(uint32_t) * 8) / 8;
	uint8_t			*data;
	size_t			len;
	t_packet		*packet;

	packet = &user->packet;
	data = packet->data + sizeof(uint32_t) + packet->cursor / 8;
	len = ft_strlen(str);
	if (len > size_left)
	{
		ft_memcpy(data, str, size_left);
		packet->cursor += 8 * size_left;
		return (str + size_left);
	}
	else
	{
		ft_memcpy(data, str, len);
		packet->cursor += 8 * len;
		return (NULL);
	}
	return (NULL);
}
