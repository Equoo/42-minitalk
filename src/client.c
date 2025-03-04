/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 15:52:58 by dderny            #+#    #+#             */
/*   Updated: 2025/03/01 16:51:59 by dderny           ###   ########.fr       */
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

int g_test = 1;

/*static int	writemessage(pid_t pid, char *message)
{
	static char	*msg;
	int			i;

	errno = 0;
	g_test = 0;
	if (!msg)
		msg = message;
	if (killandwait(pid, SIGUSR2))
		return (ERRINITNOSIG);
	if (sigwritenbr(pid, 32, (uint64_t)hash((uint8_t *)msg)))
		return (ERRINITNOSIG);
	if (sigwritenbr(pid, sizeof(size_t) * 8, ft_strlen(msg)))
		return (ERRINITNOSIG);
	i = 0;
	while (msg[i])
	{
		if (sigwritenbr(pid, sizeof(char) * 8, msg[i]))
			return (ERRINITNOSIG);
		i++;
	}
	usleep(2500);
	return (errno);
}*/

void	client_sig_handler(int sig, siginfo_t *info, void *context)
{
	static uint8_t	lastbit = 0;

	if (lastbit == SIGUSR1)
	{
		if (sig == SIGUSR1)
		{
			print_cerror("Server error", ERRINITNOSIG);
			g_test = 2;
		}
		else if (sig == SIGUSR2)
		{
			print_cerror("Server error retrying", ERRSERVERRETRY);
			g_test = 1;
			usleep(2500);
		}
	}
	lastbit = SIGUSR1;
	if (sig == SIGUSR2)
		lastbit = SIGUSR2;
	(void)context;
	(void)info;
}

#include <ft_errno.h>
int	init_client(char **argv)
{
	pid_t	pid;

	pid = ft_atoi(argv[1]);
	if (pid < 1)
		return (print_cerror("Usage", ERRINITPID));
	if (toggle_signals(TRUE))
		return (EXIT_FAILURE);
	ft_printf("Client PID: %d\n", getpid());
	/*while (g_test == 1)
		writemessage(pid, argv[2]);
	if (errno)
		return (EXIT_FAILURE);*/
	kill(pid, SIGUSR1);
	usleep(50);
	kill(pid, SIGUSR1);
	usleep(50);
	kill(pid, SIGUSR1);
	usleep(50);
	kill(pid, SIGUSR1);
	usleep(50);
	kill(pid, SIGUSR1);
	usleep(50);
	kill(pid, SIGUSR1);
	usleep(50);
	kill(pid, SIGUSR1);
	usleep(50);
	kill(pid, SIGUSR1);
	usleep(50);
	kill(pid, SIGUSR1);
	usleep(50);
	kill(pid, SIGUSR1);
	usleep(50);
	kill(pid, SIGUSR1);
	usleep(50);
	kill(pid, SIGUSR1);
	usleep(50);
	kill(pid, SIGUSR1);
	usleep(50);
	kill(pid, SIGUSR1);
	usleep(50);
	kill(pid, SIGUSR1);
	return (EXIT_SUCCESS);
}
