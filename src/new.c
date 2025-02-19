/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 22:18:56 by dderny            #+#    #+#             */
/*   Updated: 2025/02/19 17:29:24 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <libft.h>
#include <errno.h>
#include <ft_printf.h>
#include <ft_bool.h>
#include "main.h"
#include "error.h"

volatile char	sigbit;

void	sig_void(int sig)
{
	(void)sig;
}

void	sig_handler(int sig)
{
	if (sig == SIGUSR1)
		sigbit = 1;
	else if (sig == SIGUSR2)
		sigbit = 0;
}

void	sigblock(t_bool toggle)
{
	struct sigaction act;
	sigset_t sigset;

	act = (struct sigaction){0};
	sigemptyset(&sigset);
	if (toggle)
	{
		sigaddset(&sigset, SIGUSR1);
		sigaddset(&sigset, SIGUSR2);
		act.sa_handler = sig_void;
	}
	else
		act.sa_handler = sig_handler;
	act.sa_flags = 0;
	act.sa_mask = sigset;
	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);
}




/* 
**	Signal handling
*/

int		sigwait(t_bool ping)
{
	if (ping != SIGWAITWRITE && ping != SIGWAITREAD)
	{
		if (!usleep(500000))
		{
			if (ping == SIGWAITRES)
				return (ERRINITNOFIRSTSIG);
			else // so ping is egal to SIGWAITRY
				return (print_cerror("Signal number send/receive",
							ERRINITNOSIG));
		}
		else if (errno && errno != EINTR)
			return (print_error("Signal number send/receive"));
		return (0);
	}
	else if (ping == SIGWAITWRITE && usleep(5000))
		return (print_error("Signal number send/receive"));
	else if (usleep(1000000))
		return (print_error("Signal number send/receive"));
	return (0);
}

void	sigwritenbr(int pid, int bits, __int64_t n, t_sigwait ping)
{
	int			waitres;
	__int64_t	num;
	int			i;

	i = 0;
	while (i < bits)
	{
		num = (n << i) & (1 << (bits - 1));
		if (num)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		waitres = sigwait(ping);
		if (waitres == ERRINITNOFIRSTSIG)
			waitres = sigwait(SIGWAITRY);
		if (waitres)
			return ;
		i++;
	}
}

__int64_t	sigreadnbr(int pid, int bits, t_sigwait ping)
{
	int			waitres;
	__int64_t	num;
	int			i;

	i = 0;
	while (i < bits)
	{
		waitres = sigwait(ping);
		if (waitres == ERRINITNOFIRSTSIG)
			waitres = sigwait(SIGWAITRY);
		if (waitres)
			return ;
		num = (n << i) & (1 << (bits - 1));
		if (num)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		i++;
	}
}






int		loop_server(void)
{
	int		i;

	i = 0;
	while (str[i])
	{
		sigwritenbr(pid, 8, str[i], ping);
		i++;
	}
	sigwritenbr(pid, 8, '\0', ping);
}

int		init_server(void)
{
	ft_printf("Server PID: %d\n", getpid());
	return (0);
}





void	init_client(char **argv)
{
	int		pid;

	pid = ft_atoi(argv[1]);
	if (pid < 1)
		return (print_cerror("Usage", ERRINITPID));
	ft_printf("Client PID: %d\n", getpid());	
}

int		main(int argc, char **argv)
{
	int		pid;
	int		i;
	char	c;

	if (CLIENT && argc != 3)
		return (print_cerror("Usage", ERRINITARGS));
	if (signal(SIGUSR1, sig_handler) == SIG_ERR)
		return (print_cerror("Signal", ERRSIGUSR));
	if (signal(SIGUSR2, sig_handler) == SIG_ERR)
		return (print_cerror("Signal", ERRSIGUSR));
	if (CLIENT)
		return (init_client(argv));
	else
		init_server();
	while (1)
		loop_server();
	return (0);
}