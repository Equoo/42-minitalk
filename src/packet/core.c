/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:29:26 by dderny            #+#    #+#             */
/*   Updated: 2025/03/10 18:31:36 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packet.h"

#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


void	packet_sig_handler(int sig, siginfo_t *info, void *context)
{
	t_user	*user;
	uint8_t	bit;

	if (sig != SIGUSR1 && sig != SIGUSR2 && context)
		return ;
	user = getuser(info->si_pid);
	if (!user)
		return ;
	bit = (sig == SIGUSR2);
	if (user->state < USER_CONNECTED && bit)
	{
		if (user->state != USER_CONNECTING)
			kill(user->pid, SIGUSR2); // Can throw error
		user->state = USER_CONNECTED;
		net_user_connected(user);
		return ;
	}
	user->bit = bit;
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

void	user_coroutine(t_user *user)
{
	if (user->bit <= -1)
		return ;
	if (user->state == USER_WRITING)
		sending_coroutine(user);
	else if (user->state == USER_RECEIVING)
		receiving_coroutine(user);
	net_hook_loop(user);
	if (user->state == USER_CONNECTED)
	{
		user->state = USER_RECEIVING;
		user->packet = (t_packet){0};
		user->packet.error = !user->bit;
	}
	user->bit = -1;
}

void	user_sendings(t_user *user)
{
	if (user->sbit <= -1)
		return ;
	if (user->sbit)
		kill(user->pid, SIGUSR2);
	else
		kill(user->pid, SIGUSR1);
	user->sbit = -1;
}

void	net_loop(void)
{
	unsigned int	elapsed;

	while (1)
	{
		elapsed = sleep(2);
		if (!elapsed)
		{
			net_hook_loop(NULL);
			net_connection_timeout();
		}
		else
		{
			foruser(&user_coroutine);
			usleep(50);
			foruser(&user_sendings);
		}
	}
}
