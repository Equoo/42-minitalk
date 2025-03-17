/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 11:43:58 by dderny            #+#    #+#             */
/*   Updated: 2025/03/17 11:47:08 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packet.h"

#include <stdint.h>

uint32_t	hash(uint8_t *str)
{
	int			i;
	uint32_t	hash;

	i = 0;
	hash = 5381;
	while (i < PACKET_SIZE / 8)
	{
		hash = ((hash << 5) + hash) + (int)str[i];
		i++;
	}
	return (hash);
}
