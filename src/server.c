/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:14:30 by dderny            #+#    #+#             */
/*   Updated: 2025/02/26 16:37:55 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <ft_printf.h>

#include "signals.h"
#include "users.h"
#include "ft_error.h"
#include "main.h"

int	init_server(void)
{
	if (toggle_signals(TRUE))
		return (EXIT_FAILURE);
	ft_printf("Server PID: %d\n", getpid());
	return (EXIT_SUCCESS);
}

static int	readlen(t_user *user, uint8_t bit)
{
	int		state;

	state = sigreadnbr(user, bit, sizeof(size_t) * 8, &user->len);
	if (state > 0)
		return (state);
	if (state == -1)
		return (0);
	user->len += 1;
	if (user->len - 1 <= MSG_MAX)
		user->msg = malloc(user->len * sizeof(char));
	if (!user->msg)
	{
		if (user->len - 1 <= MSG_MAX)
			return (ERRMESSAGELEN);
		return (errno);
	}
	user->msg[user->len - 1] = '\0';
	return (0);
}

static int	readchar(t_user *user, uint8_t bit)
{
	static uint64_t	buffer = 0;
	int				state;

	state = sigreadnbr(user, bit, sizeof(char) * 8, &buffer);
	if (state > 0)
		return (state);
	if (state == -1)
		return (0);
	if (user->msg)
		user->msg[user->nbr] = (char)buffer;
	user->nbr++;
	buffer = 0;
	return (0);
}

static void	readmessage(t_user *user, uint8_t bit)
{
	if (!user->hash)
		sigreadnbr(user, bit, sizeof(uint64_t) * 8, &user->hash);
	else if (!user->len)
		readlen(user, bit);
	else if (user->nbr < user->len - 1)
		readchar(user, bit);
	if (errno && print_error("Signal number read"))
	{
		if (errno < 124)
			disconnect_all(TRUE);
		else
			disconnect_user(user->pid, TRUE, TRUE);
		return ;
	}
	if (user->len && user->nbr == user->len - 1)
	{
		if (user->hash != hash((uint8_t *)user->msg))
		{
			ft_printf("Message from %d failed, retry...\n", user->pid);
			disconnect_user(user->pid, TRUE, TRUE);
			return ;
		}
		ft_printf("Message from %d: %s\n", user->pid, user->msg);
		disconnect_user(user->pid, FALSE, FALSE);
	}
}

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
