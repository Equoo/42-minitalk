/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 11:45:31 by dderny            #+#    #+#             */
/*   Updated: 2025/03/18 11:32:29 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "packet.h"

#include <errno.h>
#include <stdlib.h>

void	net_quit(__attribute__((unused)) int sig)
{
	user_kickall(FALSE, FALSE);
	exit(EOWNERDEAD);
}

uint8_t	net_init(void)
{
	struct sigaction	act;
	sigset_t			sigset;

	signal(SIGQUIT, net_quit);
	signal(SIGINT, net_quit);
	errno = 0;
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
