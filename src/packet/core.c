/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:29:26 by dderny            #+#    #+#             */
/*   Updated: 2025/03/12 17:13:57 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packet.h"

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int	user_coroutine(t_user *user)
{
	if (user->bit <= -1)
		return (0);
	//ft_printf("%f| Coroutine, BIT %d, STATE %d\n", get_time(), user->bit,  user->state);
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
		//ft_printf("%f| New packet, error %d\n", get_time(), user->packet.error);
		user->sbit = 1;
	}
	user->bit = -1;
	return (0);
}

int	user_sendings(t_user *user)
{
	if (user->sbit <= -1)
		return (0);
	//ft_printf("[%d]\n", user->sbit);
	if (user->sbit)
		kill(user->pid, SIGUSR2);
	else
		kill(user->pid, SIGUSR1);
	user->sbit = -1;
	return (0);
}

int	check_sigreceived(t_user *user)
{
	if (user->bit > -1)
		return (1);
	return (0);
}

void	net_loop(void)
{
	while (1)
	{
		//ft_printf("_");
		foruser(&user_coroutine);
		usleep(10);
		foruser(&user_sendings);
		//ft_printf("-");
		int i = 0;
		while (errno != EINTR && i < 1000)
		{
			errno = 0;
			if (foruser(&check_sigreceived))
			{
				errno = EINTR;
				break ;
			}
			usleep(1000);
			//ft_printf("+");
			i++;
		}
		if (errno != EINTR)
		{
			net_hook_loop(NULL);
			net_connection_timeout();
		}
		//ft_printf("%f| Loop, errno %d\n", get_time(), errno);
		errno = 0;
	}
}

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
	//ft_printf("%f| RECEIVE, BIT %d, STATE %d\n", get_time(), bit, user->state);
	if (user->pending)
		user->pending = FALSE;
	if (user->state < USER_CONNECTED && bit)
	{
		usleep(500);
		if (user->state != USER_CONNECTING)
			kill(user->pid, SIGUSR2); // Can throw error
		user->pending = TRUE;
		user->state = USER_CONNECTED;
		net_user_connected(user);
		return ;
	}
	user->bit = bit;
	//ft_printf("%f| End handler\n", get_time());
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
