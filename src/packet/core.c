/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:29:26 by dderny            #+#    #+#             */
/*   Updated: 2025/03/04 18:18:19 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packet.h"
#include "ft_error.h"

#include <signal.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <ft_printf.h>

static void	process_packet(t_user *user, uint8_t bit)
{
	t_packet	*packet;

	packet = &user->packet;
	if (user->state != USER_RECEIVING)
	{
		user->state = USER_RECEIVING;
		packet->error = !bit;
		kill(user->pid, SIGUSR2);
		return ;
	}
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
		net_send_error(user, ERRPACKETS_DIFFER);
		return ;
	}
	ft_printf("Packet from %d: %s\n", user->pid, packet->data + sizeof(uint32_t));
	user->state = USER_NONE;
	net_receive_packet(user, packet->data + sizeof(uint32_t));
	*packet = (t_packet){0};
}

static void	send_coroutine(t_user *user, uint8_t bit)
{
	t_packet	*packet;
	uint8_t		wbytes;
	uint32_t	pos;

	packet = &user->packet;
	if (!packet->hash || !bit)
		return ;
	if (packet->cursor >= PACKET_SIZE + sizeof(uint32_t))
	{
		user->state = USER_NONE;
		user->packet = (t_packet){0};
		return ;
	}
	pos = packet->cursor / sizeof(uint8_t);
	wbytes = packet->cursor % sizeof(uint8_t);
	bit = (packet->data[pos] << wbytes) & 0x80;
	if (bit)
		kill(user->pid, SIGUSR2);
	else
		kill(user->pid, SIGUSR1);
	packet->cursor++;
}

void	packet_sig_handler(int sig, siginfo_t *info, void *context)
{
	t_user	*user;
	uint8_t	bit;

	errno = 0;
	if (sig != SIGUSR1 && sig != SIGUSR2 && context)
		return ;
	user = getuser(info->si_pid);
	if (!user)
		return ;
	bit = (sig == SIGUSR2);
	if (!user->connected && bit)
	{
		user->connected = TRUE;
		net_user_connected(user);
		kill(user->pid, SIGUSR2);
		return ;
	}
	if (user->connected)
	{
		if (user->state == USER_WRITING)
			send_coroutine(user, bit);
		else
			process_packet(user, bit);
	}
	net_hook_loop(user);
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
		{
			net_hook_loop(NULL);
			net_connection_timeout();
		}
	}
}
