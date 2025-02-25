/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:14:30 by dderny            #+#    #+#             */
/*   Updated: 2025/02/26 00:15:27 by dderny           ###   ########.fr       */
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

static int	readhash(t_user *user, uint8_t bit)
{
	int		state;

	state = sigreadnbr(user, bit, sizeof(uint64_t) * 8,
						&user->hash);
	if (state > 0)
		return (state);
	if (state == -1)
		return (0);
	return (0);
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
	user->msg[user->nbr] = (char)buffer;
	user->nbr++;
	buffer = 0;
	return (0);
}
 
void		readmessage(t_user *user, uint8_t bit)
{
	if (!user->hash)
		readhash(user, bit);
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
			ft_printf("Message from %d failed, retry...\n");
			disconnect_user(user->pid, TRUE, TRUE);
			return ;
		}
		ft_printf("Message from %d: %s\n", user->pid, user->msg);
		disconnect_user(user->pid, FALSE, FALSE);
	}
}
