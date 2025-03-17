/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dderny <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 23:58:57 by dderny            #+#    #+#             */
/*   Updated: 2025/03/17 11:42:02 by dderny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_error.h"

#include <ft_printf.h>
#include <ft_errno.h>
#include <unistd.h>

static char	*str_custom_errors(int err)
{
	if (err == ERRINITUSAGE)
		return ("./client [PID > 0] [message]");
	if (err == ERRINITLEN)
		return ("length is too long");
	if (err == ERRUSER_MAX)
		return ("Too many connected");
	if (err == ERRUSER_TIMEOUT)
		return ("Timeout");
	if (err == ERRPACKETS_ANORMAL)
		return ("Anormal reception");
	return ("Unknown error");
}

/**
** Print a custom error message
** @param category: the category of the error
** @param error: the error message
** @return 1
*/
int	print_cerror(char *category, int error)
{
	static int	err = 0;
	char		*errorstr;

	if (err)
		return (error);
	errno = error;
	err = 1;
	if (error > 133)
		errorstr = str_custom_errors(error);
	else
		errorstr = ft_strerror(error);
	ft_dprintf(STDERR_FILENO, "%s: %s\n", category, errorstr);
	return (error);
}

/**
** Print an error message
** @param category: the category of the error
** @return errno
*/
int	print_error(char *category)
{
	print_cerror(category, errno);
	return (errno);
}
