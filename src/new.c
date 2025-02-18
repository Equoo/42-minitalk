/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 22:18:56 by dderny            #+#    #+#             */
/*   Updated: 2025/02/18 22:32:55 by dderny           ###   ########.fr       */
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

volatile char	bit;

void	sig_handler(int sig)
{
	if (sig == SIGUSR1)
		bit = 1;
	else if (sig == SIGUSR2)
		bit = 0;
}

int		init_client(char **argv)
{
	int		pid;

	pid = ft_atoi(argv[1]);
	if (pid < 1)
		return (print_cerror("Usage", ERRINITPID));
	if (signal(SIGUSR1, sig_handler) == SIG_ERR)
		return (print_cerror("Signal", ERRSIGUSR1));
	return (pid);
}

int		main(int argc, char **argv)
{
	int		pid;
	int		i;
	char	c;

	if (CLIENT && argc != 3)
		return (print_cerror("Usage", ERRINITARGS));
	return (0);
}