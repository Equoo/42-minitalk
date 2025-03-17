/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:29:26 by dderny            #+#    #+#             */
/*   Updated: 2025/03/17 15:37:50 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_bool.h"
#include "ft_error.h"
#include "packet.h"

#include <errno.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

int	user_coroutine(t_user *user)
{
	if (user->bit <= -1)
		return (0);
	if (user->state == USER_WRITING && user->bit)
		sending_coroutine(user);
	else if (user->state == USER_RECEIVING)
		receiving_coroutine(user);
	else if (user->state == USER_CONNECTED || !user->bit)
	{
		user->state = USER_RECEIVING;
		user->packet = (t_packet){0};
		user->packet.error = !user->bit;
		user->sbit = 1;
	}
	if (errno && user->state == USER_WRITING && print_error(ERRWRITING))
		user_kickall(TRUE, TRUE);
	if (errno && user->state == USER_RECEIVING && print_error(ERRRECEIVING))
		user_kickall(TRUE, TRUE);
	net_hook_loop(user);
	if (errno && print_error("User coroutine"))
		user_kickall(TRUE, TRUE);
	user->bit = -1;
	return (0);
}

int	user_sendings(t_user *user)
{
	if (user->sbit <= -1)
		return (0);
	if (user->sbit)
		kill(user->pid, SIGUSR2);
	else
		kill(user->pid, SIGUSR1);
	if (errno == ESRCH)
	{
		user_disconnect(user->pid);
		errno = 0;
	}
	user->sbit = -1;
	return (0);
}

int	check_sigreceived(t_user *user)
{
	if (user->bit > -1)
	{
		errno = EINTR;
		return (1);
	}
	return (0);
}

void	net_loop(void)
{
	int	i;

	while (1)
	{
		foruser(&user_coroutine);
		foruser(&user_sendings);
		i = 0;
		while (!errno && i < 10000)
		{
			errno = 0;
			if (foruser(&check_sigreceived))
				break ;
			usleep(10);
			i++;
		}
		if (!errno)
			net_hook_loop(NULL);
		if (!errno)
			net_connection_timeout();
		if (errno == EINTR)
			errno = 0;
		if (errno && print_error("Signals processing"))
			user_kickall(TRUE, TRUE);
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
	if (user->pending)
		user->pending = FALSE;
	if (user->state < USER_CONNECTED && bit)
	{
		usleep(1000);
		if (user->state != USER_CONNECTING)
			kill(user->pid, SIGUSR2);
		if (errno == ESRCH)
			return ;
		user->pending = TRUE;
		user->state = USER_CONNECTED;
		net_hook_connect(user);
		return ;
	}
	user->bit = bit;
}
