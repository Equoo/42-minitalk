/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 22:18:56 by dderny            #+#    #+#             */ 	
/*   Updated: 2025/02/21 01:32:43 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <libft.h>
#include <errno.h>
#include <ft_printf.h>
#include <ft_bool.h>

#include "main.h"
#include "users.h"
#include "signals.h"
#include "ft_error.h"

void	server_sig_handler(int sig, siginfo_t *info, void *context)
{
	t_user	*user;
	uint8_t	bit;
	
	errno = 0;
	if (sig != SIGUSR1 && sig != SIGUSR2)
		return ;
	(void)context;
	user = getuser(info->si_pid);
	if (!user)
		return ;
	bit = 0;
	if (sig == SIGUSR2)
		bit = 1;
	if (!user->connected && bit)
	{
		user->connected = TRUE;
		kill(user->pid, SIGUSR2);
		return ;
	}
	else if (!user->connected)
		return ;
	readmessage(user, bit);
}

int		init_server(void)
{
	ft_printf("Server PID: %d\n", getpid());
	toggle_signals(TRUE);
	return (0);
}


void	sig_handler(int sig)
{
	static uint8_t	lastbit = 0;

	if (lastbit == SIGUSR1)
	{
		if (sig == SIGUSR1)
			print_cerror("Server error", ERRINITNOSIG);
		else if (sig == SIGUSR2)
			print_cerror("Server error retrying", ERRINITNOSIG);
	}
	lastbit = SIGUSR1;
	if (sig == SIGUSR2)
		lastbit = SIGUSR2;
}

int	init_client(char **argv)
{
	int		i;
	pid_t	pid;

	pid = ft_atoi(argv[1]);
	if (pid < 1)
		return (print_cerror("Usage", ERRINITPID));
	if (signal(SIGUSR1, sig_handler) == SIG_ERR && print_error("Signal initialization"))
		return (EXIT_FAILURE);
	if (signal(SIGUSR2, sig_handler) == SIG_ERR && print_error("Signal initialization"))
		return (EXIT_FAILURE);
	ft_printf("Client PID: %d\n", getpid());
	if (killandwait(pid, SIGUSR2))
		return (ERRINITNOSIG);
	sigwritenbr(pid, 64, hash((uint8_t *)argv[2]));
	sigwritenbr(pid, sizeof(size_t) * 8, ft_strlen(argv[2]));
	i = 0;
	while (argv[2][i])
	{
		sigwritenbr(pid, sizeof(char) * 8, argv[2][i]);
		i++;
	}
	return (EXIT_SUCCESS);
}

int		main(int argc, char **argv)
{
	if (CLIENT && argc != 3)
		return (print_cerror("Usage", ERRINITARGS));
	if (CLIENT)
		return (init_client(argv));
	else
		init_server();
	while (1)
		pause();
	return (EXIT_SUCCESS);
}