/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:13:05 by dderny            #+#    #+#             */
/*   Updated: 2025/03/03 19:04:41 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packet.h"
#include "ft_bool.h"
#include "ft_error.h"
#include "users.h"
#include <stdatomic.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <libft.h>
#include <ft_printf.h>

void	net_connection_timeout(void)
{
	ft_printf("Connection timeout\n");
}

static void	process_packet(t_user *user, uint8_t bit)
{
	t_packet	*packet;
	uint8_t		rbytes;

	packet = &user->packet;
	if (packet->cursor < PACKET_SIZE + sizeof(uint32_t))
	{
		packet->data[packet->cursor / sizeof(uint8_t)] = bit;
		if (packet->cursor % sizeof(uint8_t) < sizeof(uint8_t) - 1)
			packet->data[packet->cursor / sizeof(uint8_t)] <<= 1;
		packet->cursor++;
		kill(user->pid, SIGUSR2);
		return ;
	}
	packet->hash = *(uint32_t *)packet->data;
	if (packet->hash != hash(packet->data + sizeof(uint32_t)))
	{
		ft_printf("Packet from %d failed, retry...\n", user->pid);
		net_send_error(user->pid, ERRPACKETS_DIFFER);
		return ;
	}
	ft_printf("Packet from %d: %s\n", user->pid, packet->data + sizeof(uint32_t));
	net_receive_packet(user->pid, packet->data + sizeof(uint32_t));
	*packet = (t_packet){0}; 
}

void	packet_sig_handler(int sig, siginfo_t *info, void *context)
{
	t_user	*user;
	uint8_t	bit;

	errno = 0;
	if (sig != SIGUSR1 && sig != SIGUSR2)
		return ;
	user = getuser(info->si_pid);
	if (!user)
		return ;
	bit = (sig == SIGUSR2);
	if (!user->connected && bit)
	{
		user->connected = TRUE;
		kill(user->pid, SIGUSR2);
		return ;
	}
	if (user->connected)
		process_packet(user, bit);
}

uint8_t	net_init(void)
{
	struct sigaction	act;
	sigset_t			sigset;

	act = (struct sigaction){0};
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGUSR1);
	sigaddset(&sigset, SIGUSR2);
	act.sa_sigaction = packet_sig_handler;
	act.sa_flags = SA_SIGINFO;
	act.sa_mask = sigset;
	if (sigaction(SIGUSR1, &act, NULL))
		return (errno);
	if (sigaction(SIGUSR2, &act, NULL))
		return (errno);
	return (0);
}

void	net_loop(void)
{
	unsigned int elapsed;

	while (1)
	{
		elapsed = sleep(2);
		if (!elapsed)
			net_connection_timeout();
		
	}
}

t_packet	*net_new_packet(t_user *user, t_bool error)
{
	t_packet	packet;

	user->state = USER_RECEIVING;
	user->packet = (t_packet){0};
	user->packet.error = error;
	return (&user->packet);
}

const char	*net_write_string(t_packet *packet, const char *str)
{
	const size_t	size_left = (PACKET_SIZE - packet->cursor) / sizeof(uint8_t);
	uint8_t			*data;
	size_t			len;
	
	data = packet->data + sizeof(uint32_t);
	len = ft_strlen(str);
	if (len > size_left)
	{
		memcpy(data, str, size_left);
		packet->cursor += sizeof(uint8_t) * size_left;
		return (str + size_left);
	}
	else
	{
		memcpy(data, str, len);
		packet->cursor += sizeof(uint8_t) * len;
		return (NULL);
	}
	return (NULL);
}

uint8_t	net_write_uint32(t_packet *packet, const uint32_t nbr)
{
	uint8_t	*data;

	if (packet->cursor + sizeof(uint32_t) > PACKET_SIZE)
		return (1);
	data = packet->data + sizeof(uint32_t);
	*(uint32_t *)(data + packet->cursor / sizeof(uint8_t)) = nbr;
	packet->cursor += sizeof(uint32_t);
	return (0);
}

int	net_send_packet(int pid, t_packet *packet)
{
	if (!packet->hash)
	{	
		packet->hash = hash(packet->data + sizeof(uint32_t));
		*(uint32_t *)packet->data = packet->hash;
		packet->cursor = 0;
	}
	if (kill(pid, SIGUSR1))
		return (errno);
	return (0);
}
