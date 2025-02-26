/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:52:58 by dderny            #+#    #+#             */
/*   Updated: 2025/02/26 16:42:49 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <libft.h>
#include <ft_printf.h>
#include <ft_bool.h>

#include "main.h"
#include "signals.h"
#include "ft_error.h"

static int	writemessage(pid_t pid, char *message)
{
	static char	*msg;
	int			i;

	errno = 0;
	if (!msg)
		msg = message;
	if (killandwait(pid, SIGUSR2))
		return (ERRINITNOSIG);
	sigwritenbr(pid, 64, hash((uint8_t *)msg));
	sigwritenbr(pid, sizeof(size_t) * 8, ft_strlen(msg));
	i = 0;
	while (msg[i])
	{
		sigwritenbr(pid, sizeof(char) * 8, msg[i]);
		i++;
	}
	return (errno);
}

void	client_sig_handler(int sig, siginfo_t *info, void *context)
{
	static uint8_t	lastbit = 0;

	if (lastbit == SIGUSR1)
	{
		if (sig == SIGUSR1)
			print_cerror("Server error", ERRINITNOSIG);
		else if (sig == SIGUSR2)
		{
			print_cerror("Server error retrying", ERRSERVERRETRY);
			writemessage(info->si_pid, NULL);
		}
	}
	lastbit = SIGUSR1;
	if (sig == SIGUSR2)
		lastbit = SIGUSR2;
	(void)context;
}

int	init_client(char **argv)
{
	pid_t	pid;

	pid = ft_atoi(argv[1]);
	if (pid < 1)
		return (print_cerror("Usage", ERRINITPID));
	if (toggle_signals(TRUE))
		return (EXIT_FAILURE);
	ft_printf("Client PID: %d\n", getpid());
	writemessage(pid, argv[2]);
	while (1)
		pause();
	if (errno)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
