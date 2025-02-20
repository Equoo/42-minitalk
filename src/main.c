/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 15:08:56 by dderny            #+#    #+#             */
/*   Updated: 2025/02/19 19:34:43 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <libft.h>
#include <errno.h>
#include <ft_printf.h>
#include "main.h"
#include "error.h"

int	process_close(void)
{
	if (errno != 11 && errno && print_error("Initialization"))
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
	return (EXIT_SUCCESS);
}

void	do_nothing2(int signum, siginfo_t *info, void *context)
{
	(void)signum;
	ft_printf("Siginfo %d\n", info->si_pid);
	(void)context;	
}

void	do_nothing(int signum)
{
	(void)signum;
}

void	block(void)
{
	struct sigaction act;
	sigset_t sigset;

	act = (struct sigaction){0};
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGUSR1);
	sigaddset(&sigset, SIGUSR2);
	act.sa_sigaction = do_nothing2;
	act.sa_flags = SA_SIGINFO;
	act.sa_mask = sigset;
	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);
}

void	handle_signal(int signum);
void	unblock(void)
{
	struct sigaction act;
	sigset_t sigset;

	act = (struct sigaction){0};
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGUSR1);
	sigaddset(&sigset, SIGUSR2);
	act.sa_handler = handle_signal;
	act.sa_flags = 0;
	act.sa_mask = sigset;
	sigaction(SIGUSR1, &act, NULL);
	sigaction(SIGUSR2, &act, NULL);
}

volatile int client = 0;
volatile int letter = 0;
volatile int bits = 0;
volatile char str[1024];

void	handle_signal(int signum)
{	
	static unsigned int i = 0;
	static unsigned int clpid = 0;
	int bit;

	block();
	bit = 0;
	if (signum == SIGUSR2)
		bit = 1;
	
	//ft_printf("Signal received %d, %d, %d, %d\n", signum, bits, client, clpid);

	if (!client)
	{
		clpid += bit;
		if (bits < 31)
			clpid <<= 1;
		bits++;
		if (bits == 32)
		{
			client = clpid;
			clpid = 0;
			bits = 0;
			ft_printf("Client connected %d\n", client);
		}
		//unblock();
		return ;
	}

	usleep(300);
		
	letter += bit;
	if (bits < 7)
		letter <<= 1;
	bits++;

	if (bits == 8)
	{
		ft_printf("Add %c, %d\n", letter, letter);
		str[i] = letter;
		i++;
		bits = 0;
		if (!letter)
		{
			ft_printf("Message: %s\n", str);
			i = 0;
			letter = 0;
			unblock();
			kill(client, SIGUSR2);
			client = 0;
			return ;
		}
		letter = 0;
	}
	//unblock();
	kill(client, SIGUSR2);
}

int	main(int argc, char **argv)
{
	if (!CLIENT && !SERVER)
		return (print_cerror("Initialization", ERRINITVARS));
	if (CLIENT && argc != 3)
		return (print_cerror("Initialization", ERRINITARGS));
	
	unsigned long i = 0;
	if (SERVER)
	{
		ft_printf("PID: %d\n", getpid());
		signal(SIGUSR1, handle_signal);
		signal(SIGUSR2, handle_signal);
		while (1)
		{
			if (pause() == -1)
			{
				if (errno == EINTR)
					continue ;
				return (print_error("Initialization"));
			}
		}
	}
	else
	{
		signal(SIGUSR2, do_nothing);
		ft_printf("PID: %d\n", getpid());
		int j = 0;
		while (j < 32)
		{
			int cbit = getpid();
			cbit <<= j;
			cbit >>= 31;
			if (cbit)
				kill(ft_atoi(argv[1]), SIGUSR2);
			else
				kill(ft_atoi(argv[1]), SIGUSR1);
			if (usleep(5000) && errno != EINTR)
				return (print_error("Initialization"));
			j++;
		}
		while (argv[2][i])
		{
			char c = argv[2][i];
			
			int j = 0;
			while (j < 8)
			{
				char cbit = c;
				cbit <<= j;
				cbit >>= 7;
				if (cbit)
					kill(ft_atoi(argv[1]), SIGUSR2);
				else
					kill(ft_atoi(argv[1]), SIGUSR1);
				if (!usleep(10000000) || errno != EINTR)
					return (print_cerror("Initialization", ERRINITNOSIG));
				j++;
			}
			ft_printf("Sent to %d, %c, %i\n", ft_atoi(argv[1]), argv[2][i], argv[2][i]);
			i++;
		}
		i = 0;
		while (i < 8)
		{
			kill(ft_atoi(argv[1]), SIGUSR1);
			if (!usleep(10000000) || errno != EINTR)
				return (print_cerror("Initialization", ERRINITNOSIG));
			i++;
		}
		ft_printf("Signal sent to %d\n", ft_atoi(argv[1]));
	}
	return (0);
}