/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 11:56:03 by dderny            #+#    #+#             */
/*   Updated: 2025/02/25 23:54:11 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <ft_bool.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#include "signals.h"
#include "ft_error.h"

void	sig_action_void(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	(void)sig;
}

int		killandwait(pid_t pid, uint8_t sig)
{
	kill(pid, sig);
	if (!usleep(500000))
	{
		kill(pid, sig);
		if (!errno && !usleep(500000))
			errno = ERRINITNOSIG;
	}
	if (errno == EINTR)
		errno = 0;
	return (errno);
}

int		sigwritenbr(int pid, int bits, size_t n)
{
	int			waitres;
	size_t		num;
	uint8_t		sig;
	int			i;

	i = 0;
	while (i < bits)
	{
		usleep(50 * LAG_FACTOR);
		num = n & (1ul << (bits - 1 - i));
		sig = SIGUSR1;
		if (num)
			sig = SIGUSR2;
		if (killandwait(pid, sig))
			return (ERRINITNOSIG);
		i++;
	}
	return (errno);
}

int		sigreadnbr(t_user *user, uint8_t bit, uint8_t bits, uint64_t *n)
{
	int		state;

	user->buffer += bit;
	if (user->rbytes == bits - 1)
	{
		*n = user->buffer;
		user->rbytes = 0;
		user->buffer = 0;
		state = 0;
	}
	else
	{
		user->buffer <<= 1;
		user->rbytes++;
		state = -1;
	}
	if (usleep(50 * LAG_FACTOR) && errno != EINTR)
		return (errno);
	if (errno == EINTR)
		errno = 0;
	if (kill(user->pid, SIGUSR2))
		return (errno);
	return (state);
}

int		toggle_signals(t_bool toggle)
{
	struct sigaction act;
	sigset_t sigset;

	act = (struct sigaction){0};
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGUSR1);
	sigaddset(&sigset, SIGUSR2);
	if (!toggle)
		act.sa_sigaction = sig_action_void;
	else
		act.sa_sigaction = server_sig_handler;
	act.sa_flags = SA_SIGINFO;
	act.sa_mask = sigset;
	if (sigaction(SIGUSR1, &act, NULL))
		return (print_error("signal initialization"));
	if (sigaction(SIGUSR2, &act, NULL))
		return (print_error("signal initialization"));
	return (EXIT_SUCCESS);
}